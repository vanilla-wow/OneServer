/*
* Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef _ELUNA_BINDING_H
#define _ELUNA_BINDING_H

#include "Common.h"
#include "LuaEngine.h"
#include "ElunaUtility.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
};

class ElunaBind : public ElunaUtil::RWLockable
{
public:
    struct Binding
    {
        int functionReference;
        bool isTemporary;
        uint32 remainingShots;
        Eluna& E;

        Binding(Eluna& _E, int funcRef, uint32 shots) :
            functionReference(funcRef),
            isTemporary(shots != 0),
            remainingShots(shots),
            E(_E)
        {
        }

        // Remove our function from the registry when the Binding is deleted.
        ~Binding()
        {
            luaL_unref(E.L, LUA_REGISTRYINDEX, functionReference);
        }
    };
    typedef std::vector<Binding*> FunctionRefVector;
    typedef UNORDERED_MAP<int, FunctionRefVector> EventToFunctionsMap;

    Eluna& E;
    const char* groupName;

    ElunaBind(const char* bindGroupName, Eluna& _E) : E(_E), groupName(bindGroupName)
    {
    }

    virtual ~ElunaBind()
    {
        Clear();
    }

    // unregisters all registered functions and clears all registered events from the bindings
    virtual void Clear() { };
};

template<typename T>
class EventBind : public ElunaBind
{
public:
    EventBind(const char* bindGroupName, Eluna& _E) : ElunaBind(bindGroupName, _E)
    {
    }

    // unregisters all registered functions and clears all registered events from the bind std::maps (reset)
    void Clear() override
    {
        WriteGuard guard(GetLock());

        for (EventToFunctionsMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
        {
            FunctionRefVector& funcrefvec = itr->second;
            for (FunctionRefVector::iterator it = funcrefvec.begin(); it != funcrefvec.end(); ++it)
                delete *it;
            funcrefvec.clear();
        }
        Bindings.clear();
    }

    void Clear(uint32 event_id)
    {
        WriteGuard guard(GetLock());

        for (FunctionRefVector::iterator itr = Bindings[event_id].begin(); itr != Bindings[event_id].end(); ++itr)
            delete *itr;
        Bindings[event_id].clear();
    }

    // Pushes the function references and updates the counters on the binds and erases them if the counter would reach 0
    void PushFuncRefs(lua_State* L, int event_id)
    {
        WriteGuard guard(GetLock());

        for (FunctionRefVector::iterator it = Bindings[event_id].begin(); it != Bindings[event_id].end();)
        {
            FunctionRefVector::iterator it_old = it++;
            Binding* binding = (*it_old);

            lua_rawgeti(L, LUA_REGISTRYINDEX, binding->functionReference);

            if (binding->isTemporary)
            {
                binding->remainingShots--;
                if (binding->remainingShots == 0)
                {
                    delete binding;
                    Bindings[event_id].erase(it_old);
                }
            }
        }

        if (Bindings[event_id].empty())
            Bindings.erase(event_id);
    };

    void Insert(int eventId, int funcRef, uint32 shots) // Inserts a new registered event
    {
        WriteGuard guard(GetLock());
        Bindings[eventId].push_back(new Binding(E, funcRef, shots));
    }

    // Checks if there are events for ID
    bool HasEvents(T eventId)
    {
        ReadGuard guard(GetLock());

        if (Bindings.empty())
            return false;
        if (Bindings.find(eventId) == Bindings.end())
            return false;
        return true;
    }

    EventToFunctionsMap Bindings; // Binding store Bindings[eventId] = {(funcRef, counter)};
};

template<typename T>
class EntryBind : public ElunaBind
{
public:
    typedef UNORDERED_MAP<uint32, EventToFunctionsMap> EntryToEventsMap;

    EntryBind(const char* bindGroupName, Eluna& _E) : ElunaBind(bindGroupName, _E)
    {
    }

    // unregisters all registered functions and clears all registered events from the bindmap
    void Clear() override
    {
        WriteGuard guard(GetLock());

        for (EntryToEventsMap::iterator itr = Bindings.begin(); itr != Bindings.end(); ++itr)
        {
            EventToFunctionsMap& funcmap = itr->second;
            for (EventToFunctionsMap::iterator it = funcmap.begin(); it != funcmap.end(); ++it)
            {
                FunctionRefVector& funcrefvec = it->second;
                for (FunctionRefVector::iterator i = funcrefvec.begin(); i != funcrefvec.end(); ++i)
                    delete *i;
                funcrefvec.clear();
            }
            funcmap.clear();
        }
        Bindings.clear();
    }

    void Clear(uint32 entry, uint32 event_id)
    {
        WriteGuard guard(GetLock());

        for (FunctionRefVector::iterator itr = Bindings[entry][event_id].begin(); itr != Bindings[entry][event_id].end(); ++itr)
            delete *itr;
        Bindings[entry][event_id].clear();
    }

    // Pushes the function references and updates the counters on the binds and erases them if the counter would reach 0
    void PushFuncRefs(lua_State* L, int event_id, uint32 entry)
    {
        WriteGuard guard(GetLock());

        for (FunctionRefVector::iterator it = Bindings[entry][event_id].begin(); it != Bindings[entry][event_id].end();)
        {
            FunctionRefVector::iterator it_old = it++;
            Binding* binding = (*it_old);

            lua_rawgeti(L, LUA_REGISTRYINDEX, binding->functionReference);

            if (binding->isTemporary)
            {
                binding->remainingShots--;
                if (binding->remainingShots == 0)
                {
                    delete binding;
                    Bindings[entry][event_id].erase(it_old);
                }
            }
        }

        if (Bindings[entry][event_id].empty())
            Bindings[entry].erase(event_id);

        if (Bindings[entry].empty())
            Bindings.erase(entry);
    };

    void Insert(uint32 entryId, int eventId, int funcRef, uint32 shots) // Inserts a new registered event
    {
        WriteGuard guard(GetLock());
        Bindings[entryId][eventId].push_back(new Binding(E, funcRef, shots));
    }

    // Returns true if the entry has registered binds
    bool HasEvents(T eventId, uint32 entryId)
    {
        ReadGuard guard(GetLock());

        if (Bindings.empty())
            return false;

        EntryToEventsMap::const_iterator itr = Bindings.find(entryId);
        if (itr == Bindings.end())
            return false;

        return itr->second.find(eventId) != itr->second.end();
    }

    bool HasEvents(uint32 entryId)
    {
        ReadGuard guard(GetLock());

        if (Bindings.empty())
            return false;

        return Bindings.find(entryId) != Bindings.end();
    }

    EntryToEventsMap Bindings; // Binding store Bindings[entryId][eventId] = {(funcRef, counter)};
};

#endif

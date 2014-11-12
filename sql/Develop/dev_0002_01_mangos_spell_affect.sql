ALTER TABLE develop_db_version CHANGE COLUMN required_dev_0001_01_mangos_develop_db_version required_dev_0002_01_mangos_spell_affect BIT;

DELETE FROM `spell_affect` WHERE `entry` = 36563;

INSERT INTO `spell_affect` (`entry`, `effectId`, `SpellFamilyMask`) VALUES
(36563,1,0x1062612030F),
(36563,2,0x1062612030F);

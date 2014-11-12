ALTER TABLE dev_db_version CHANGE COLUMN required_dev_0006_01_mangos_mangos_string required_dev_0006_02_mangos_command BIT;

DELETE FROM `command` WHERE NAME = 'nameannounce';
INSERT INTO `command` VALUES
('nameannounce',1,'Syntax: .nameannounce $MessageToBroadcast\r\n\r\nSend a global message to all players online in chat log with colored sender\'s name.');

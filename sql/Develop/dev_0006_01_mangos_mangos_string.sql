ALTER TABLE dev_db_version CHANGE COLUMN required_dev_0005_01_mangos_mangos_string required_dev_0006_01_mangos_mangos_string BIT;

DELETE FROM `mangos_string` WHERE entry IN (67, 68, 69);
INSERT INTO `mangos_string` VALUES
(67,'|c1f40af20 <Mod>|cffff0000[%s]|c1f40af20 announce:|cffffff00 %s|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(68,'|c1f40af20 <GM>|cffff0000[%s]|c1f40af20 announce:|cffffff00 %s|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(69,'|c1f40af20 <Admin>|cffff0000[%s]|c1f40af20 announce:|cffffff00 %s|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

ALTER TABLE dev_db_version CHANGE COLUMN required_dev_0004_01_mangos_dev_db_version required_dev_0005_01_mangos_mangos_string BIT;

DELETE FROM `mangos_string` WHERE `entry` = 1700;

INSERT INTO `mangos_string` VALUES (1700, '|cffffcc00[Server]: |cff00ff00%s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

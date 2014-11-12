ALTER TABLE develop_db_version CHANGE COLUMN required_dev_0002_01_mangos_spell_affect required_dev_0003_01_mangos_instance_template BIT;

ALTER TABLE `instance_template` ADD COLUMN `allowMount` tinyint(1) NOT NULL DEFAULT '0' AFTER `reset_delay`;

UPDATE instance_template SET allowMount = 1 WHERE map IN (209, 269, 309, 509, 534, 560, 568, 580);

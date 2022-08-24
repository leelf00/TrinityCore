-- Felmyst
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=25267;
UPDATE `creature_template` SET `modelid2`=22838 WHERE `entry`=25038;
DELETE FROM `creature_text` WHERE `creatureid`=25038 AND `groupid`=7;
INSERT INTO `creature_text` (`creatureid`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(25038, 7, 0, 'Felmyst takes a deep breath.', 41, 0, 100, 0, 0, 0, 20021, 0, 'felmyst - BREATH_EMOTE');
DELETE FROM `creature_template_movement` WHERE `CreatureId` = 25038;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`) VALUES
(25038, 0, 0, 1, 0);
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|0x40000256 WHERE `entry`=25038;

--UPDATE `creature_template` SET `flags_extra`='1' WHERE  `entry`=25038;

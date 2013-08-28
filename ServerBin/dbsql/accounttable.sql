delimiter $$

CREATE TABLE `account` (
  `Account` varchar(50) NOT NULL,
  `Password` varchar(45) NOT NULL,
  `CharacterName` varchar(45) NOT NULL,
  `Sex` tinyint(4) NOT NULL DEFAULT '0',
  `UserUID` int(10) unsigned NOT NULL,
  `nCoin` int(10) unsigned DEFAULT '50000',
  `nDiamoned` int(10) unsigned zerofill NOT NULL DEFAULT '0000000000',
  `nTitle` tinyint(3) unsigned zerofill NOT NULL DEFAULT '000',
  `nVipLevel` tinyint(3) unsigned zerofill NOT NULL DEFAULT '000',
  `nSex` tinyint(3) unsigned zerofill NOT NULL,
  `nDefaulPhotoID` smallint(5) unsigned DEFAULT '1',
  `nUserDefinePhotoID` int(10) unsigned zerofill DEFAULT NULL,
  `nYeastodayWinCoin` int(10) unsigned zerofill NOT NULL DEFAULT '0000000000',
  `nSingleWinMost` int(10) unsigned zerofill NOT NULL DEFAULT '0000000000',
  `nWinTimes` smallint(5) unsigned zerofill NOT NULL DEFAULT '00000',
  `nLoseTimes` smallint(5) unsigned zerofill NOT NULL DEFAULT '00000',
  `nQQNumber` int(10) unsigned zerofill NOT NULL DEFAULT '0000000000',
  `strSigure` varchar(145) DEFAULT NULL,
  PRIMARY KEY (`Account`,`UserUID`),
  UNIQUE KEY `UserUID_UNIQUE` (`UserUID`),
  UNIQUE KEY `Account_UNIQUE` (`Account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8$$


-- --------------------------------------------------------
-- 主機:                           192.168.56.104
-- 伺服器版本:                        10.1.48-MariaDB - MariaDB Server
-- 伺服器作業系統:                      Linux
-- HeidiSQL 版本:                  11.0.0.5919
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- 傾印 stock 的資料庫結構
CREATE DATABASE IF NOT EXISTS `stock` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `stock`;

-- 傾印  資料表 stock.ExecReport 結構
CREATE TABLE IF NOT EXISTS `ExecReport` (
  `NID` float NOT NULL,
  `ExecPrice` double(22,0) NOT NULL,
  `Side` int(1) NOT NULL,
  PRIMARY KEY (`NID`,`Side`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.ExecReport 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `ExecReport` DISABLE KEYS */;
/*!40000 ALTER TABLE `ExecReport` ENABLE KEYS */;

-- 傾印  資料表 stock.InitData 結構
CREATE TABLE IF NOT EXISTS `InitData` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Date` varchar(50) NOT NULL DEFAULT '0' COMMENT '日期',
  `Price_Now` double(22,0) NOT NULL DEFAULT '0' COMMENT '現價',
  `Price_Start` double NOT NULL COMMENT '開盤價',
  `Price_End` double NOT NULL COMMENT '收盤價',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.InitData 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `InitData` DISABLE KEYS */;
INSERT INTO `InitData` (`ID`, `Date`, `Price_Now`, `Price_Start`, `Price_End`) VALUES
	(1, '20210425', 60, 60.5, 60.5);
/*!40000 ALTER TABLE `InitData` ENABLE KEYS */;

-- 傾印  資料表 stock.Order 結構
CREATE TABLE IF NOT EXISTS `Order` (
  `NID` float NOT NULL DEFAULT '0',
  `OrderPrice` double NOT NULL,
  `Symbol` varchar(50) NOT NULL,
  `UserID` float NOT NULL,
  `Side` int(1) NOT NULL,
  PRIMARY KEY (`NID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.Order 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `Order` DISABLE KEYS */;
/*!40000 ALTER TABLE `Order` ENABLE KEYS */;

-- 傾印  資料表 stock.ProductList 結構
CREATE TABLE IF NOT EXISTS `ProductList` (
  `product_id` varchar(50) NOT NULL,
  `transaction_flag` enum('Y','N') NOT NULL,
  `note` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`product_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.ProductList 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `ProductList` DISABLE KEYS */;
INSERT INTO `ProductList` (`product_id`, `transaction_flag`, `note`) VALUES
	('KKC', 'Y', 'kow kow coin');
/*!40000 ALTER TABLE `ProductList` ENABLE KEYS */;

-- 傾印  資料表 stock.User 結構
CREATE TABLE IF NOT EXISTS `User` (
  `user` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  PRIMARY KEY (`user`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 正在傾印表格  stock.User 的資料：~0 rows (近似值)
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` (`user`, `password`) VALUES
	('0324027', '123');
/*!40000 ALTER TABLE `User` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;

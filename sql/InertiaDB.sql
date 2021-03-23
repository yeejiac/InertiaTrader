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

-- 傾印  資料表 stock.Order 結構
CREATE TABLE IF NOT EXISTS `Order` (
  `NID` float NOT NULL DEFAULT '0',
  `OrderID` varchar(50) NOT NULL,
  `OrderPrice` double NOT NULL,
  `Symbol` varchar(50) NOT NULL,
  `UserID` float NOT NULL,
  PRIMARY KEY (`NID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 取消選取資料匯出。

-- 傾印  資料表 stock.test1 結構
CREATE TABLE IF NOT EXISTS `test1` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` text,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- 取消選取資料匯出。

-- 傾印  資料表 stock.User 結構
CREATE TABLE IF NOT EXISTS `User` (
  `user` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  PRIMARY KEY (`user`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- 取消選取資料匯出。

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;

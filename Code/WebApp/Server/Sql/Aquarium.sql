-- phpMyAdmin SQL Dump
-- version 4.0.6
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: May 21, 2014 at 01:01 PM
-- Server version: 5.5.33
-- PHP Version: 5.5.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Aquarium`
--

-- --------------------------------------------------------

--
-- Table structure for table `Aquarium`
--

CREATE TABLE if not exists  `Aquarium` (
  `aquaID` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  PRIMARY KEY (`aquaID`)
);

--
-- Table structure for table `Sensor`
--

CREATE TABLE if not exists  `Sensor` (
  `sensorID` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  PRIMARY KEY (`sensorID`)
);

--
-- Table structure for table `Users`
--

CREATE TABLE if not exists  `Users` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `password` text NOT NULL,
  PRIMARY KEY (`userID`)
);

--
-- Table structure for table `Device`
--

CREATE TABLE if not exists  `Device` (
  `devID` int(11) NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `status` text NOT NULL,
  PRIMARY KEY (`devID`)
);

--
-- Table structure for table `Lecture`
--

CREATE TABLE if not exists  `Lecture` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `aquaID` int(11) NOT NULL references `aquaID`,
  `sensorID` int(11) NOT NULL references `sensorID`,
  `data` int(11) NOT NULL,
  `value` int(11) NOT NULL,
  PRIMARY KEY (`ID`)
);

/*inserimento degli account*/
INSERT INTO Users (name, password) VALUES ('gianni', 'Viva.le.2005');
INSERT INTO Users (name, password) VALUES ('pampa', 'Viva.le.2005');
INSERT INTO Users (name, password) VALUES ('tina', 'Viva.le.2005');

/*inserimento dei device*/
INSERT INTO Device (name, status) VALUES ('Filters', "OFF");
INSERT INTO Device (name, status) VALUES ('Resistors', "OFF");
INSERT INTO Device (name, status) VALUES ('Blower', "OFF");
INSERT INTO Device (name, status) VALUES ('Pump', "OFF");

/*inserimento dei sensori*/
INSERT INTO Sensor (name) VALUES ('AirTemp');
INSERT INTO Sensor (name) VALUES ('WaterTemp');
INSERT INTO Sensor (name) VALUES ('WaterLevel');
INSERT INTO Sensor (name) VALUES ('Humidity');
INSERT INTO Sensor (name) VALUES ('Lux');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;


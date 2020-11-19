-- phpMyAdmin SQL Dump
-- version 5.0.3
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost
-- Temps de generació: 10-11-2020 a les 19:05:34
-- Versió del servidor: 10.4.14-MariaDB
-- Versió de PHP: 7.4.11

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de dades: `prova`
--

-- --------------------------------------------------------

--
-- Estructura de la taula `lectura_rfid`
--

CREATE TABLE `lectura_rfid` (
  `id` int(11) NOT NULL,
  `lectura` varchar(12) NOT NULL,
  `data` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Bolcament de dades per a la taula `lectura_rfid`
--

INSERT INTO `lectura_rfid` (`id`, `lectura`, `data`) VALUES
(1, ' 0a 75 f0 0b', '2020-11-10 00:00:00'),
(2, ' 02 e2 76 1a', '2020-11-10 00:00:00'),
(3, ' 0a 75 f0 0b', '2020-11-10 18:52:37'),
(4, ' 02 e2 76 1a', '2020-11-10 18:52:43'),
(5, ' d9 42 c0 87', '2020-11-10 18:52:52'),
(6, ' 0a 75 f0 0b', '2020-11-10 18:52:54'),
(7, ' 0a 75 f0 0b', '2020-11-10 18:52:56'),
(8, ' 0a 75 f0 0b', '2020-11-10 18:52:57'),
(9, ' 0a 75 f0 0b', '2020-11-10 18:53:07'),
(10, ' eb aa 49 1b', '2020-11-10 18:54:16'),
(11, ' 02 e2 76 1a', '2020-11-10 18:54:20'),
(12, ' 0a 75 f0 0b', '2020-11-10 18:58:13'),
(13, ' d9 42 c0 87', '2020-11-10 18:58:20'),
(14, ' 0a 75 f0 0b', '2020-11-10 19:00:42'),
(15, ' 0a 75 f0 0b', '2020-11-10 19:02:06'),
(16, ' 02 e2 76 1a', '2020-11-10 19:02:17'),
(17, ' eb aa 49 1b', '2020-11-10 19:02:26'),
(18, ' d9 42 c0 87', '2020-11-10 19:02:36'),
(19, ' 02 e2 76 1a', '2020-11-10 19:02:51');

--
-- Índexs per a les taules bolcades
--

--
-- Índexs per a la taula `lectura_rfid`
--
ALTER TABLE `lectura_rfid`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT per les taules bolcades
--

--
-- AUTO_INCREMENT per la taula `lectura_rfid`
--
ALTER TABLE `lectura_rfid`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

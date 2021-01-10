BEGIN
SET @user_id := (SELECT id FROM users WHERE rfid = new.lectura);
SET @last_time := (SELECT fi FROM cefire where (user_id = @user_id AND fi = "00:00:00" AND `data` = date(now()))); IF(@last_time = '00:00:00') THEN
UPDATE `cefire` SET `fi` = time(now()) where (cefire.user_id = @user_id AND `data` = date(now()) and cefire.fi = "00:00:00");
else
INSERT INTO `cefire` (`user_id`, `data`, `inici`, `fi`) VALUES (@user_id, date(now()), time(now()), '00:00:00');
end if;
SET @last_time := NULL;
END

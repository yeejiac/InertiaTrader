CHANGE MASTER TO 
  MASTER_HOST='database_master',
  MASTER_PORT=3306,
  MASTER_USER='slave_user',
  MASTER_PASSWORD='password',
  MASTER_LOG_FILE='mysql-bin.000001',
  MASTER_LOG_POS=529;

START SLAVE;

SHOW SLAVE STATUS;
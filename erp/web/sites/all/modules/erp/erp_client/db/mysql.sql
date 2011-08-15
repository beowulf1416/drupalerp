create table erp_clients (
	client_id int auto_increment not null,
	name varchar(500) not null,
	address varchar(1000) null,
	primary key (client_id)
)
engine innodb;
create table erp_clients if not exists (
	id int auto_increment not null comment 'pk, client id',
	name varchar(500) not null,
	address varchar(1000) null,
	description varchar(4000) null,
	enabled int not null default 0 comment '0 = enabled, otherwise false'
	created date not null,
	primary key (id)
)
engine innodb
comment 'clients information';
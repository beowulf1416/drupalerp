/*
create table erp_contacts (
	id int not null auto_increment comment 'pk, contact id',
	uid int comment 'fk, users.uid',
	last_name varchar(500) not null,
	first_name varchar(500) not null,
	middle_name varchar(500),
	prefix varchar(100) comment 'title or salutation (eg. Dr.)',
	suffix varchar(100) comment 'title or salutation (eg. MD, RN)',
	primary key (id),
	unique (uid),
	foreign key (uid) references users(uid)
)
engine innodb
comment 'contacts information table';

create table erp_contact_types (
	id int not null auto_increment comment 'pk, contact type id',
	name varchar(100) comment 'contact information type name',
	enabled int not null default 0 '0=enabled, otherwise disabled',
	primary key (id),
	unique (name)
)
engine innodb
comment 'contact types, eg. phone, fax';

create table erp_contact_infos (
	id int not null auto_increment comment 'pk, contact information id',
	contact_type_id int not null,
	info varchar(1000) not null,
	primary key (id),
	foreign key (contact_type_id) references erp_contact_types(id)
)
engine innodb
comment 'contact information for a contact';
*/
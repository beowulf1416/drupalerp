create table erp_account_types (
	id int not null comment 'pk, account type id',
	name varchar(100) not null,
	primary key (id)
)
engine innodb
comment 'account types';

create table erp_accounts (
	id int not null auto_increment comment 'pk, account id',
	account_type_id int not null comment 'fk, erp_account_types.id',
	client_id int not null comment 'fk, erp_clients.id',
	name varchar(1000) not null,
	description varchar(4000) null,
	is_summary int not null default 0 comment '1 if this entry is a parent account/summary'
	created date not null,
	primary key (id),
	foreign key (client_id) references erp_clients(id)
)
engine innodb
comment 'accounts';

create table erp_account_hierarchy (
	client_id int not null comment 'fk, erp_clients.id',
	parent_account_id int not null comment 'fk, erp_accounts.id',
	account_id int not null comment 'fk, erp_accounts.id',
	unique (client_id, parent_account_id, account_id),
	foreign key (client_id) references erp_clients(id),
	foreign key (parent_account_id) references erp_accounts(id),
	foreign key (account_id) references erp_accounts(id)
)
engine innodb
comment 'account hierarchy'

create table erp_account_balances (
	account_id int not null comment 'pk, fk, erp_accounts.id',
	updated date not null,
	debit_amt decimal(12,3) not null default 0,
	credit_amt decimal(12,3) not null default 0,
	primary key (account_id),
	foreign key (account_id) references erp_accounts(id)
)
engine innodb
comment 'account balances';

create table erp_account_balances_snapshots_meta (
	id int not null auto_increment comment 'pk, snapshot id',
	client_id int not null comment 'fk, erp_clients.id'
	created date not null,
	primary key (id)
)
engine innodb
comment 'account balances snapshots metadata';

create table erp_account_balances_snapshots (
	snapshot_id int not null comment 'fk, erp_account_balances_snapshots_meta.id',
	client_id int not null comment 'fk, erp_clients.id',
	account_id int not null comment 'fk, erp_accounts.id',
	debit_amt decimal(12,3) not null default 0,
	credit_amt decimal(12,3) not null default 0,
	foreign key (snapshot_id) references erp_account_balances_snapshots_meta(id),
	foreign key (client_id) references erp_clients(id),
	foreign key (account_id) references erp_accounts(id)
)
engine innodb
comment 'account balances snapshots';
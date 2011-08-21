
insert into erp_accounts (id, name) values 
(1,'Assets'),
(2,'Liabilities'),
(3,'Equity'),
(4,'Revenue'),
(5,'Expense');

alter table erp_accounts
add foreign key (client_id) references erp_clients(id),
add foreign key (type_id) references erp_account_types(id);

alter table erp_account_balances
add foreign key (account_id) references erp_accounts(id),
add foreign key (client_id) references erp_clients(id);

alter table erp_account_balances_snapshots_metadata
add foreign key (client_id) references erp_clients(id);

alter table erp_account_balances_snapshots
add foreign key (client_id) references erp_clients(id),
add foreign key (account_id) references erp_accounts(id),
add foreign key (snapshot_id) references erp_account_balances_snapshots_metadata(id);
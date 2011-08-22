
insert into erp_accounts (id, name) values 
(1,'Assets'),
(2,'Liabilities'),
(3,'Equity'),
(4,'Revenue'),
(5,'Expense');

alter table erp_accounts
add constraint fk_erp_accounts_1 foreign key (client_id) references erp_clients(id),
add constraint fk_erp_accounts_2 foreign key (type_id) references erp_account_types(id);

alter table erp_account_balances
add constraint fk_erp_account_bal_1 foreign key (account_id) references erp_accounts(id),
add constraint foreign key (client_id) references erp_clients(id);

alter table erp_account_balances_snapshots_metadata
add constraint fk_erp_acct_bal_ss_meta_1 foreign key (client_id) references erp_clients(id);

alter table erp_account_balances_snapshots
add constraint fk_erp_acct_bal_ss_1 foreign key (client_id) references erp_clients(id),
add constraint fk_erp_acct_bal_ss_2 foreign key (account_id) references erp_accounts(id),
add constraint fk_erp_acct_bal_ss_3 foreign key (snapshot_id) references erp_account_balances_snapshots_metadata(id);
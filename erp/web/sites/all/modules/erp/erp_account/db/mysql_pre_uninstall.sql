


alter table erp_accounts
drop foreign key fk_erp_accounts_1,
drop foreign key fk_erp_accounts_2;

alter table erp_account_balances
drop foreign key fk_erp_account_bal_1,
drop foreign key fk_erp_account_bal_2;

alter table erp_account_balances_snapshots_metadata
drop foreign key fk_erp_acct_bal_ss_meta_1;

alter table erp_account_balances_snapshots
drop foreign key fk_erp_acct_bal_ss_1,
drop foreign key fk_erp_acct_bal_ss_2,
drop foreign key fk_erp_acct_bal_ss_3;
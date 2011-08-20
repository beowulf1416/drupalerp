alter table erp_contacts 
add foreign key (client_id) references erp_clients(id),
add foreign key (uid) references users(uid);

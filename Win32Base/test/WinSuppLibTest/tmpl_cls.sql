$if(First)$

insert into Module$$ ([Id], [Name], [Ver], [VerBack])

	values(kmid${Module}, '${Module}', ${ModVer}, ${ModVerBack})

#if kmid${Module} == kmidCellar

insert into Class$$ ([Id], [Mod], [Base], [Abstract], [Name])

	values(kclidCmObject, kmidCellar, kclidCmObject, 1, 'CmObject')

#endif



$endif$

insert into Class$$ ([Id], [Mod], [Base], [Abstract], [Name])

	values(kclid${Class}, kmid${Module}, kclid${Base}, $if(Abstract)1$else0$endif, '${Class}')



DbgNuke(${Class}, table)

DbgGo()

print 'creating table [${Class}]'

go

create table [${Class}] (

	[Id] int constraint [_PK_${Class}] primary key clustered,

		constraint [_FK_${Class}_Id] foreign key ([Id]) references [${Base}] ([Id]),

$foreach$

${PropSqlFldDfn}$

$endfor$

)

go



DbgNuke(TR_${Class}_TStmp, trigger)

DbgGo()



create trigger [TR_${Class}_TStmp] on [${Class}] for update

as

	update	CmObject

	set 	UpdDttm = getdate()

	from 	CmObject co join inserted i on co.[Id] = i.[Id]

go



$if(!Abstract)$

exec DefineCreateProc$$ kclid${Class}, 'Create_${Class}'

exec DefineCreateOwnedProc$$ kclid${Class}, 'CreateOwned_${Class}'

go

$endif$




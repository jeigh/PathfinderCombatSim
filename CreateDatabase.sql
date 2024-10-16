create database PCS
go
use PCS
go
create table AttackScoreCalculation (
	pkid int primary key identity not null,
	AttackersAttackBonus int not null, 
	DefendersAC int not null, 
	MinimumCrit int not null, 
	CritMultiplier int not null, 
	AttackRoll int not null,
	DamageDiceCount int not null, 
	DamageDieMean float not null,
	ExpectedDamageResult float  
)
go
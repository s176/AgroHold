#ifndef _AgroHold_PopEnergo_h_
#define _AgroHold_PopEnergo_h_

class PanEnergo : public WithPanelLayout<ParentCtrl> {
	
	GridCtrl grid;
	Sql sql;
	Id ID;
	String TableName;
	EditString edName;
	EditDouble edBalance;
	EditDouble edYearNorm;
	EditDouble edAmort;
	EditDouble edTOR; 
		
    void Insert()
    {
		try
		{	
			String sqlrequest;
			sql.Execute("SELECT MAX(ID) FROM " + TableName);
			sql.Fetch();
			int x = sql[0];
			int newid;
			if (x > 0) 
				newid = x + 1;
			else
				newid = 1;
			sql.Execute("SELECT * FROM [" + TableName + "] LIMIT 1");
			for(int i = 1; i < sql.GetColumns(); i++)
			{	
				sqlrequest += AsString(grid(i)) + "'";
				if(i != sql.GetColumns() - 1)
				 	sqlrequest +=", '";
			}		
			sqlrequest = "INSERT INTO [" + TableName + "] VALUES ('" + AsString(newid) + "', '" + sqlrequest + ")";
			sql.Execute(sqlrequest);
			grid(0) = newid;
		}
		catch(SqlExc &e)
		{
			grid.CancelInsert();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
	void Update()
	{
		try
		{
			String sqlrequest;
			sql.Execute("SELECT * FROM [" + TableName + "] LIMIT 1");
			for(int i = 1; i < sql.GetColumns(); i++)
			{	
				sqlrequest += sql.GetColumnInfo(i).name + "='" + AsString(grid(i)) + "' ";
				if(i != sql.GetColumns() - 1)
				 	sqlrequest +=",";
			}
			sql.Execute("UPDATE [" + TableName + "] SET " + sqlrequest + "WHERE ID=" + AsString(grid(0)));
		}
		catch(SqlExc &e)
		{
			grid.CancelUpdate();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	void Remove()
	{
		try
		{
			if (Prompt("Агроплан", CtrlImg::question(), "Видалити запис?", "Так", "Ні"))
			{
				sql.Execute("DELETE FROM [" + TableName + "] WHERE ID=" + AsString(grid(0)));
			}
			else
			{
				grid.CancelRemove();
			}
		}
		catch(SqlExc &e)
		{
			grid.CancelRemove();
			Exclamation("[* " + DeQtfLf(e) + "]");
		}
	}
	
public:        	
	
	void Populate()
	{
		grid.Clear();
		sql.Execute("SELECT * FROM " + TableName + " ORDER BY Name");
		int row = 0;
		while(sql.Fetch())
		{
			grid.Add(sql[0]);
			for (int i = 0; i < sql.GetColumns(); i++)		
			{
				grid.Set(row, i, sql[i]);
			}
		row++;
		}	
	}

    typedef PanEnergo CLASSNAME;
 	PanEnergo()
 	{
 		CtrlLayout(*this);	
 		Add(grid.SizePos());
 		TableName = "Energo";
		grid.AddIndex(ID);
		grid.AddColumn("Марка трактора", 200).Edit(edName);
		grid.AddColumn("Балансова вартість, тис. грн.", 200).AlignRight().Edit(edBalance).SetConvert(Single<ConvDouble>());		
		grid.AddColumn("Річна зайнятість, годин", 150).AlignRight().Edit(edYearNorm).SetConvert(Single<ConvDouble>());		
		grid.AddColumn("Норма амортизації, %", 150).AlignRight().Edit(edAmort).SetConvert(Single<ConvDouble>());		
		grid.AddColumn("Норма ремонту, %", 150).AlignRight().Edit(edTOR).SetConvert(Single<ConvDouble>());		
		
		grid.WhenInsertRow = THISBACK(Insert);
		grid.WhenUpdateRow = THISBACK(Update);
		grid.WhenRemoveRow = THISBACK(Remove);
		grid.Appending().Removing().Editing().Accepting().Canceling().Indicator().Searching().Absolute().SetToolBar();
	};
};


#endif

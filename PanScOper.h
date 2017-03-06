#ifndef _AgroHold_PanScOper_h_
#define _AgroHold_PanScOper_h_

#include <ScatterCtrl/ScatterCtrl.h>

class PanScOper : public WithPanelLayout<ParentCtrl> 
{   	
	public:
	int Horizont;
	int cyear;
	Label lbl;
	DropGrid yearselect;
	Sql sql;
	Id ID;
	ScatterCtrl scatter;
	Vector<double> sx, sy;
	
	
	void Populate()
	{
		// установим горизонт планирования
		sql.Execute("SELECT * FROM Project WHERE Param = 'Horizont'");
		if(sql.Fetch())
			Horizont = sql[1];
		// установим начальный год
		sql.Execute("SELECT * FROM Project WHERE Param = 'Year'");
		if(sql.Fetch())
			cyear = sql[1];
		// заполнение выпадающего списка планового года
		yearselect.Clear();
		for (int i = 1; i <= Horizont; i++)
			yearselect.Add(i, cyear + i);
		//yearselect.SetData(1);
		//yearselect.Refresh();
		//YearSelected();
	}
	
	void YearSelected()
	{
		sql.Execute("SELECT * FROM Graphik"); //WHERE Year = " + AsString(yearselect.GetData()));
		int row = 0;
		Date date;
		while(sql.Fetch())
		{
			StrToDate(date, AsString(sql[1]));
			int selectedyear = yearselect.GetData(); 
			if (date.year == (selectedyear + cyear))
			{
				//grid.Add(sql[0]);
				for (int i = 0; i < sql.GetColumns(); i++)		
				{
					//grid.Set(row, i, sql[i]);
				}
				
				row++;
			}
		}
		scatter.RemoveAllSeries();
		sy.Clear();
		sx.Clear();
		sy << 14 + rand()%10 << 25 + rand()%15 << 19 + rand()%30 << 50 - rand()%40 << 16 + rand()%5 << 45 + rand()%3;
		sx << 1 << 2 << 3 << 4 << 5 << 8;
		scatter.AddSeries(sx, sy).Legend("Series").PlotStyle<StaggeredSeriesPlot>().Dash("").Opacity(0.5).NoMark().Fill().Stroke(3, LtRed());
		scatter.Refresh();
	}
	
	void MyFormatX(String& s, int i, double d)
	{
		int selectedyear = yearselect.GetData();
		if (!IsNull(selectedyear))
		{
			s = Format("%Mon", fround(d)); 
			if (!s.IsEmpty())
				s << "\n" << AsString(selectedyear + cyear);
		}
	}
	
	void MyFormatY(String& s, int i, double d)
	{
		s = Format("%Mon", fround(i)); 
		if (!s.IsEmpty())
			s << "\n2011";
	}
	
    typedef PanScOper CLASSNAME;
 	PanScOper()
 	{		
 		CtrlLayout(*this);
 		 			
		// график
		scatter.SetFrame(InsetFrame());
		scatter.SetPlotAreaLeftMargin(80).SetPlotAreaRightMargin(30).SetPlotAreaTopMargin(30).SetPlotAreaBottomMargin(90).SetLabelX(t_("Місяць")).SetLabelsFont(SansSerifZ(11));
		scatter.ShowContextMenu().SetMouseHandling(true, false);
		scatter.SetRange(12, 50).SetMajorUnits(1, 10);
		//scatter.SetLabelY("Кількість агрегатів");
		
		scatter.cbModifFormatX = THISBACK(MyFormatX);		    
  		scatter.cbModifFormatY = THISBACK(MyFormatY);
  
  		Add(lbl.SetLabel(t_("Рік плану")).LeftPosZ(12, 64).TopPosZ(12, 21));
 		Add(yearselect.LeftPosZ(84, 96).TopPosZ(12, 21));
 		Add(scatter.HSizePosZ(0, 0).VSizePosZ(40, 0));
 			
		// Таблиця випадаючого списку планового року					
		yearselect.AddColumn("ID").Width(0);
		yearselect.AddColumn("Рік", 100);
		yearselect.SetKeyColumn(0);
		yearselect.SetValueColumn(1);

		yearselect.WhenAction = THISBACK(YearSelected);
		
	};
};


#endif

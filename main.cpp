#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include <DropGrid/DropGrid.h>
#include <plugin/sqlite3/lib/sqlite3.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define IMAGEFILE <AgroHold/AgroHold.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>

#include <plugin/sqlite3/lib/sqlite3.h>
#include <plugin/sqlite3/Sqlite3.h>

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <AgroHold/AgroHold.sch>

#include <Sql/sch_schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>

#define LAYOUTFILE <AgroHold/AgroHold.lay>
#include <CtrlCore/lay.h>

// Библиотека линейного программирования
#include "lp_lib.h"

// Подключение классов по конвертации данных для грида
#include "Convert.h"

// Подключение отдельных форм
#include "PanCult.h"
#include "PanType.h"
#include "PanFields.h"
#include "PanPop.h"
#include "PanEnergo.h"
#include "PanMash.h"
#include "PanMatType.h"
#include "PanMaterials.h"
#include "PanOper.h"
#include "PanTech.h"
#include "PanSiv.h"
#include "PanPosiv.h"
#include "PanZbir.h"
#include "PanGraph.h"
#include "PanProject.h"
#include "PanScOper.h"

// Главный класс программы
class AgroHold : public WithAgroHoldLayout<TopWindow> {
public:
	typedef AgroHold CLASSNAME;

	void TreeClick();

	// Описание меню и вкладок
	MenuBar menu;
	void Exit() 
	{
		if (PromptOKCancel("Завершити програму?"))
			Break();
	}
	void Compact() 
	{
		(PromptOKCancel("Компонувати програму?"));
	}
	void Base(Bar& bar) 
	{
		bar.Add("Компонувати", THISBACK(Compact));
		bar.Add("Завершити", THISBACK(Exit));
	}
		
	void MainMenu(Bar& bar) 
	{
		bar.Add("База даних", THISBACK(Base));
	}
	
	// описание контролов
	Splitter split_main;				// Главный сплит програмы в центре
	TreeCtrl   tree_main;				// Главное дерево слева
	WithPanelLayout<ParentCtrl> panel;	// Главная панель справа
	
	// обєявление подключаемых форм
	PanProject pProject; // форма параметров проекта: название, текущий год, ставки дисконтирования
	PanCult pCult;	// форма культур
	PanType pType;	// типи культур
	PanFlds pFlds;	// поля
	PanPop  pPop;	// попередники
	PanEnergo  pEnergo;// енергомашини
	PanMachinery pMash;// с.-г. машини
	PanMatType pMatType;// тип матеріалу
	PanMaterials pMaterials;// матеріали
	PanOper pOper;// операції
	PanTech pTech;// технології
	PanSiv pSiv;// сівозміни
	PanPosiv pPosiv;// отчет посевные площади
	PanZbir pZbir;// отчет валовой сбор
	PanGraph pGraph;// Графік операцій
	PanScOper pScOper;// Графік операцій
	
	// Указатель на контрол, который надо будет удалить с панели при смене форм
	Ctrl *ctrl;
	AgroHold();
};

void AgroHold::TreeClick()
{
	//выбор узла дерева
	TreeCtrl::Node n;
	n = tree_main.GetNode(tree_main.GetCursor());
	String p = AsString(n.key);
	panel.RemoveChild(ctrl);	// удаляем текущий контрол с панели
	if (p == "Проект")
	{
		panel.Add(pProject.SizePos());
		pProject.Populate();
		tree_main.SetFocus();
		ctrl = &pProject;
	}
	if (p == "Культури")
	{
		panel.Add(pCult.SizePos());
		pCult.Populate();
		tree_main.SetFocus();
		ctrl = &pCult;
	}
	if (p == "Типи культур")
	{
		panel.Add(pType.SizePos());
		pType.Populate();
		tree_main.SetFocus();
		ctrl = &pType;
	}
	if (p == "Поля")
	{
		panel.Add(pFlds.SizePos());
		pFlds.Populate();
		tree_main.SetFocus();
		ctrl = &pFlds;
	}
	if (p == "Попередники")
	{
		panel.Add(pPop.SizePos());
		pPop.Populate();
		tree_main.SetFocus();
		ctrl = &pPop;
	}
	if (p == "Енергомашини")
	{
		panel.Add(pEnergo.SizePos());
		pEnergo.Populate();
		tree_main.SetFocus();
		ctrl = &pEnergo;
	}
	if (p == "С.-г. машини")
	{
		panel.Add(pMash.SizePos());
		pMash.Populate();
		tree_main.SetFocus();
		ctrl = &pMash;
	}
	if (p == "Тип ТМЦ")
	{
		panel.Add(pMatType.SizePos());
		pMatType.Populate();
		tree_main.SetFocus();
		ctrl = &pMatType;
	}
	if (p == "Матеріали")
	{
		panel.Add(pMaterials.SizePos());
		pMaterials.Populate();
		tree_main.SetFocus();
		ctrl = &pMaterials;
	}
	if (p == "Операції")
	{
		panel.Add(pOper.SizePos());
		pOper.Populate();
		tree_main.SetFocus();
		ctrl = &pOper;
	}
	if (p == "Технології")
	{
		panel.Add(pTech.SizePos());
		pTech.Populate();
		tree_main.SetFocus();
		ctrl = &pTech;
	}
	if (p == "Сівозміни")
	{
		panel.Add(pSiv.SizePos());
		pSiv.Populate();
		tree_main.SetFocus();
		ctrl = &pSiv;
	}
	if (p == "Площі посівів, га")
	{
		panel.Add(pPosiv.SizePos());
		pPosiv.Populate();
		tree_main.SetFocus();
		ctrl = &pPosiv;
	}
	if (p == "Валовий збір, т")
	{
		panel.Add(pZbir.SizePos());
		pZbir.Populate();
		tree_main.SetFocus();
		ctrl = &pZbir;
	}
	if (p == "Календарний план")
	{
		panel.Add(pGraph.SizePos());
		pGraph.Populate();
		tree_main.SetFocus();
		ctrl = &pGraph;
	}
	if (p == "Графік робіт")
	{
		panel.Add(pScOper.SizePos());
		pScOper.Populate();
		tree_main.SetFocus();
		ctrl = &pScOper;
	}
}

AgroHold::AgroHold() // Конструктор класса
{
	CtrlLayout(*this, "АгроПлан");
	
	Icon(Images::SmallIcon());
	LargeIcon(Images::LargeIcon());
	
	// добавление меню на форму
	AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	
	// создание контрола на основе панели
	CtrlLayout(panel);
	
	// добавление главного сплита на форму
	split_main.Horz(tree_main, panel);
	Add(split_main.SizePos());
	split_main.SetPos(1500);
	
	// изображения на дереве
	CtrlImg::Set(CtrlImg::I_treeplus, Images::tree_plus());
	CtrlImg::Set(CtrlImg::I_treeminus, Images::tree_minus());
	
	// заполнение главного дерева
	tree_main.SetRoot(Images::Folder(), "Проект"); 			// Встановлюємо кореневу папку 0
		tree_main.Add(0,Images::Folder(), "Довідники");			// 1
		tree_main.Add(1,Images::File(), "Типи культур");		// 2
		tree_main.Add(1,Images::File(), "Культури");			// 3
		tree_main.Add(1,Images::File(), "Поля");				// 4
		tree_main.Add(1,Images::File(), "Попередники");			// 5
		tree_main.Add(1,Images::File(), "Енергомашини");		// 6
		tree_main.Add(1,Images::File(), "С.-г. машини");		// 7
		tree_main.Add(1,Images::File(), "Тип ТМЦ");				// 8
		tree_main.Add(1,Images::File(), "Матеріали");			// 9
		tree_main.Add(1,Images::File(), "Операції");			// 10
		tree_main.Add(1,Images::File(), "Технології");			// 11
	tree_main.Add(0,Images::Folder(), "Операційні плани");		// 12
		tree_main.Add(12,Images::Grid(), "Сівозміни");			// 13
		tree_main.Add(12,Images::Report(), "Площі посівів, га");// 14
		tree_main.Add(12,Images::Report(), "Валовий збір, т");	// 15
		tree_main.Add(12,Images::Clndr(), "Календарний план");// 16	
		tree_main.Add(12,Images::Diag(), "Графік робіт");		// 17	
		tree_main.Add(12,Images::Diag(), "Потреба у техніці");	// 18
	tree_main.Add(0,Images::Folder(), "Фінансові плани");		// 19
		tree_main.Add(19,Images::File(), "Доходи і витрати");	// 20
		tree_main.Add(19,Images::File(), "Рух коштів");			// 21
			
	// раскрываем дерево
	tree_main.Open(0);
	tree_main.Open(1);
	tree_main.Open(12);
	tree_main.Open(19);
	
	// действие на дереве
	tree_main.WhenAction = THISBACK(TreeClick);
	tree_main.WhenLeftDouble = THISBACK(TreeClick);
	
	// начальная панель
	panel.Add(pProject.SizePos());
	pProject.Populate();
	tree_main.SetFocus();
	ctrl = &pProject;
	
	Sizeable().Zoomable().Maximize();	// форма изменяемая и с кнопками
}

GUI_APP_MAIN
{
	bool nodb = false;
	Sqlite3Session db;
	FileIn fi("Database.db");
	if(fi.IsError() || fi.GetSize() <= 0)
		nodb = true;
	fi.Close();	

	if(!db.Open(ConfigFile("Database.db")))
	{
		Exclamation(t_("Не вдалось створити чи відкрити файл бази даних"));
		return;
	}
	SQL = db;
	if(nodb)
	{
		SqlSchema sch(SQLITE3);
		StdStatementExecutor se(db);
		All_Tables(sch);
		if(sch.ScriptChanged(SqlSchema::UPGRADE))
			Sqlite3PerformScript(sch.Upgrade(), se);
		if(sch.ScriptChanged(SqlSchema::ATTRIBUTES))
			Sqlite3PerformScript(sch.Attributes(), se);
		if(sch.ScriptChanged(SqlSchema::CONFIG))
		{
			Sqlite3PerformScript(sch.ConfigDrop(), se);
			Sqlite3PerformScript(sch.Config(), se);
		}
	}
	SetLanguage(SetLNGCharset(GetSystemLNG(), CHARSET_UTF8));
	AgroHold ah;
    ah.Run();		// запуск приложеня 
}
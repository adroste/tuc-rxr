#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "StateDev.h"
#include "../GameEditor.h"
#include "../../../Framework/UI/UIItemLister.h"
#include "../../../Framework/UI/Editor/UIContainerMaterial.h"
#include "../../../Framework/UI/UIMenuBar.h"
#include "../../../Framework/UI/Editor/UIDialogLights.h"
#include "../../../Framework/UI/UIWindowContainerHolder.h"
#include "../../../Framework/UI/UIDialogContainerHolder.h"
#include "../../../Framework/UI/Editor/UIContainerMapSetup.h"
#include "../../../Framework/UI/Editor/UIWindowBuckets.h"
#include "../../../Framework/UI/Editor/UIContainerBucketFiles.h"
#include "../../Shared/MapLoader.h"

class StateEditor : public GameState
{
	// dont want to write this every time...
	using BucketImport = UIDialogContainerHolder<UIContainerBucketFiles>;
public:
	StateEditor()
		:
		/*m_uiList({&m_btnBack, &m_wndMaterial, &m_menu, &m_dlgLights, &m_dlgMapSetup,
			&m_wndBucks}),*/
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		m_dlgLights(m_editor, false, *this),
		m_menu(Drawing::getFont(Font::Style::Headline, Font::Size::M)),
		m_wndMaterial(std::unique_ptr<UIContainerMaterial>(new UIContainerMaterial(true)), true),
		m_dlgMapSetup(UIDialog::Buttons::OKCancel, false, *this),
		m_wndBucks(false)
	{
		// TODO fix z index thing
		m_menu->setZIndex(100);

		m_menu->addSection("File");
		m_menu->addItem("File", "New", [this](const std::string&)
		               {
			               m_editor.reset();
		               });
		m_menu->addItem("File", "Open", [this](const std::string&)
		{
			openMap();
		});
		m_menu->addItem("File", "Save");
		m_menu->addItem("File", "Save as", [this](const std::string&)
		               {
			               saveMap();
		               });
		m_menu->addItem("File", "Import Buckets", [this](const std::string&)
		               {
			               if (m_pDlgBuckImport.get()) return;
						   m_pDlgBuckImport = owner_ptr<BucketImport>(new BucketImport(UIDialog::Buttons::OKCancel, true, *this));
			               //m_pDlgBuckImport->registerMe(this);
			               m_pDlgBuckImport->adjustToContainer();
			               //m_pDlgBuckImport->center();

			               m_pDlgBuckImport->show();

						   addWindow(m_pDlgBuckImport.getRef());

			               m_pDlgBuckImport->setOnResultCallback([this](UIDialog*)
				               {
					               handleBucketImport();
				               });
		               });

		m_menu->addSection("Edit");
		m_menu->addItem("Edit", "Undo");
		m_menu->addItem("Edit", "Redo");

		m_menu->addSection("Map");
		m_menu->addItem("Map", "Lights", [this](const std::string&)
		{
			//sortReceivers(); // TODO maybe add broadcaster reference to uilist? for better focus setting
			//m_dlgLights.center();
			m_dlgLights->show();
		});
		m_menu->addItem("Map","Material",[this](const std::string&)
		{
			m_wndMaterial->show();
		});
		m_menu->addItem("Map", "Dimension", [this](const std::string&)
		               {
			               m_dlgMapSetup->show();
		               });
		m_menu->addItem("Map", "Buckets", [this](const std::string&)
		               {
			               m_wndBucks->show();
		               });

		m_editor.registerMe(this);

		m_btnBack->adjustToFontHeadline();
		//m_btnBack.setOrigin({10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f)});
		m_btnBack->setZIndex(1);

		// material list
		(*m_wndMaterial)->orderItems();
		(*m_wndMaterial)->adjustToItems();
		m_wndMaterial->setZIndex(2);
		m_wndMaterial->adjustToContainer();
		//m_wndMaterial.setOrigin({800,100});

		m_wndBucks->setZIndex(3);
		//m_wndBucks.setOrigin({10,50});

		m_dlgMapSetup->adjustToContainer();
		//m_dlgMapSetup.center();

		//m_uiList.registerAll(this);

		m_wndBucks->setOnBucketChangeCallback([this](UIWindowBuckets* pb)
			{
				m_editor.setCurrentBlocks(pb->getCurDesc());
			});

		m_editor.setOnCaptureCallback([this](bool getCapture)
			{
				if (getCapture)
				{
					// set highest z index
					m_editor.setZIndex(INT_MAX);
					//sortReceivers();
				}
				else
				{
					// set index 0
					m_editor.setZIndex(0);
					//sortReceivers();
				}
			});

		m_dlgMapSetup->setOnResultCallback([this](UIDialog* dlg)
			{
				if (dlg->getResult() == UIDialog::Result::OK)
				{
					m_editor.setMapdim((*m_dlgMapSetup)->getMapdim());
				}
			});
		(*m_wndMaterial)->setOnToBucketCallback([this](CubeDesc c)
			{
				m_wndBucks->addToBucket(c);
			});
		
		addWindow(m_btnBack.getRef(), Anchor::Bottom | Anchor::Left);
		addWindow(m_menu.getRef(), Anchor::Top);	
		addWindow(m_wndBucks.getRef(), Anchor::Left);
		addWindow(m_wndMaterial.getRef(), Anchor::Right);
		(*m_wndMaterial)->quickFickAddColorPickerDialogToWindowManager(*this);
		addWindow(m_dlgLights.getRef(), 0, PointF(0.0f), true);
		addWindow(m_dlgMapSetup.getRef(), 0, PointF(0.0f), true);
	}

	virtual ~StateEditor() override
	{
	}

	virtual bool update(float dt) override
	{
		if (m_btnBack->isClicked(true))
			setNextState(TransitionState::Discard);

		if (m_pDlgBuckImport && m_pDlgBuckImport->getResult() != UIDialog::Result::None)
		{
			// delete and unregister (unregister will be automatically with owner_ptr)
			m_pDlgBuckImport.reset();
		}

		return true;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		m_editor.draw(draw, dt);

		//m_uiList.draw(draw);
		drawWindows(draw);
	}

	void saveMap()
	{
		std::string filename = "sample_scene";

		MapLoader::MapInfo i;
		i.lights = m_editor.getLights();
		i.ambient = m_editor.getAmbientColor();
		i.nChunks = m_editor.getMapChunkSize();
		i.chunkCubes = m_editor.getCubeDescs();

		MapLoader::save(filename, i);
	}

	void openMap()
	{
		std::string filename = "sample_scene";

		MapLoader l(filename);
		if(l.isOpen())
		{
			const auto& i = l.getInfo();
			m_editor.loadMap(i);
			/*m_editor.reset();
			m_editor.setMapdim({ i.nChunks.x * MapChunk::SIZE, i.nChunks.y * MapChunk::SIZE, Map::DEPTH });
			size_t idx = 0;
			for(const auto& c : i.chunkCubes)
			{
				PointS p = i.nChunks.fromIndex(idx);
				m_editor.loadCubes(c, p.x, p.y);
				idx++;
			}
			m_editor.updateLights(i.ambient, i.lights);*/
		}
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		GameState::keyUp(s);
		return true;
	}

	virtual bool charDown(char c) override
	{
		GameState::charDown(c);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		GameState::mouseMove(mpos, mdiff, handled);
		return true;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseUp(mpos, button);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		GameState::wheel(mpos, amount);
		return true;
	}

	// Events
	virtual void onResize() override
	{
		GameState::onResize();
		m_menu->orderItems();
	}

	void handleBucketImport()
	{
		if (m_pDlgBuckImport->getResult() == UIDialog::Result::OK)
		{
			// load buckets
			for (const auto& n : (*m_pDlgBuckImport)->getSelectedNames())
			{
				// load buckets
				MaterialLoader l("saves/editor/buckets/" + n);
				if (l.isOpen())
				{
					m_wndBucks->addBucket();
					for (const auto& c : l.getDesc())
					{
						m_wndBucks->addToBucket(c);
					}
				}
			}
		}
	}

private:
	//UIObjectList m_uiList;

	GameEditor m_editor;

	refable<UIButtonText> m_btnBack;
	refable<UIDialogLights> m_dlgLights;
	refable<UIMenuBar> m_menu;
	// material list
	refable<UIWindowContainerHolder<UIContainerMaterial>> m_wndMaterial;
	refable<UIDialogContainerHolder<UIContainerMapSetup>> m_dlgMapSetup;
	owner_ptr<BucketImport> m_pDlgBuckImport;
	refable<UIWindowBuckets> m_wndBucks;
};

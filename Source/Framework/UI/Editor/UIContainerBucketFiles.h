#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UICheckBox.h"
#include "../../../Utility/Directory.h"

class UIContainerBucketFiles : public UIItemLister
{
public:
	UIContainerBucketFiles()
		:
		UIItemLister(),
		m_lblTitle(getFont(),"Buckets:")
	{
		
		addToList(&m_lblTitle, nullptr);
		auto names = GetDirectoryFilenames("saves/editor/buckets/");
		for(const auto& n : names)
		{
			auto lbl = std::unique_ptr<UILabel>(new UILabel(getFont(), n));
			auto box = std::unique_ptr<UICheckBox>(new UICheckBox(false));
			addToList(lbl.get(), box.get());
			m_filenames.push_back(std::move(lbl));
			m_checks.push_back(std::move(box));
		}

		setCellPadding(8.0f);
		setWallPadding(5.0f);
		adjustToItems();
		orderItems();
	}
	std::vector<std::string> getSelectedNames() const
	{
		std::vector<std::string> v;
		for(size_t i = 0; i < m_checks.size(); i++)
		{
			if(m_checks[i]->getValue())
			{
				v.push_back(m_filenames[i]->getText());
			}
		}
		return v;
	}
private:
	static Font& getFont()
	{
		return Drawing::getFont(Font::Style::Text, Font::Size::M);
	}
private:
	UILabel m_lblTitle;
	std::vector<std::unique_ptr<UILabel>> m_filenames;
	std::vector<std::unique_ptr<UICheckBox>> m_checks;
};

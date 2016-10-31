#pragma once
#include "../UIItemLister.h"
#include "../UILabel.h"
#include "../UICheckBox.h"
#include "../../../Utility/Directory.h"

class UIContainerBucketFiles : public UIItemLister
{
public:
	UIContainerBucketFiles(bool show)
		:
		UIItemLister(show),
		m_lblTitle(getFont(),"Buckets:")
	{
		
		addToList(m_lblTitle.getRef(), nullptr);
		auto names = GetDirectoryFilenames("saves/editor/buckets/");
		for(const auto& n : names)
		{
			auto lbl = owner_ptr<UILabel>(new UILabel(getFont(), n));
			auto box = owner_ptr<UICheckBox>(new UICheckBox(false));
			addToList(lbl.getRef(), box.getRef());
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
	refable<UILabel> m_lblTitle;
	std::vector<owner_ptr<UILabel>> m_filenames;
	std::vector<owner_ptr<UICheckBox>> m_checks;
};

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		if (!PlayLayer::get()){
			return;
		}
		// level
		auto Level = PlayLayer::get()->m_level;
		// add tag
		getChildByID("level-name")->setTag(1);
		getChildByID("center-button-menu")->setTag(2);
		getChildByID("music-slider")->setTag(3);
		getChildByID("sfx-slider")->setTag(4);
		getChildByID("music-label")->setTag(5);
		getChildByID("sfx-label")->setTag(6);

		if (Level->isPlatformer()){
			getChildByID("play-time")->setTag(10);
		} else{
			getChildByID("normal-progress-bar")->setTag(7);
			getChildByID("practice-progress-bar")->setTag(8);
			getChildByID("normal-progress-label")->setTag(9);
			getChildByID("practice-progress-label")->setTag(10);
			getChildByID("normal-mode-label")->setTag(11);
			getChildByID("practice-mode-label")->setTag(12);
		}


		getChildByID("left-button-menu")->setTag(13);
		getChildByID("right-button-menu")->setTag(14);
		getChildByID("bottom-button-menu")->setTag(15);

		// utility
		auto Size = CCDirector::sharedDirector()->getWinSize();

		// chat btn
		auto menu_rb = static_cast<CCMenu*>(getChildByID("right-button-menu"));

		auto img_options = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
		img_options->setScale(0.8);
		auto btn_options = CCMenuItemSpriteExtra::create(img_options, this, menu_selector(OptionsLayer::onOptions));
		menu_rb->addChild(btn_options);

		auto img_comment = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
		img_comment->setScale(0.65);
		auto btn_comment = CCMenuItemSpriteExtra::create(img_comment, this, menu_selector(MyPauseLayer::onLevelInfo));
		menu_rb->addChild(btn_comment);

		auto img_quest = CCSprite::create("GJ_questBtn.png"_spr);
		img_quest->setScale(0.55);
		auto btn_quest = CCMenuItemSpriteExtra::create(img_quest, this, menu_selector(CreatorLayer::onChallenge));
		menu_rb->addChild(btn_quest);

		menu_rb->setLayout(
			ColumnLayout::create()
				->setGap(4.f)
				->setAutoScale(true)
				->setAxisAlignment(AxisAlignment::End)
				->setCrossAxisAlignment(AxisAlignment::Center)
   		);
		menu_rb->updateLayout();

		// creator name
		auto author = Level->m_creatorName;
		if (Level->m_levelType == GJLevelType::Local) author = "RobTop"; // official
		std::string by = "By ";
		auto tlb_author = CCLabelBMFont::create((by + author).c_str(), "goldFont.fnt");
		tlb_author->setScale(0.8);

		auto btn_author = CCMenuItemSpriteExtra::create(tlb_author, this, menu_selector(MyPauseLayer::onViewProfile));
	
		auto menu_author = CCMenu::create();
		menu_author->setPosition(CCPoint(Size.width / 2, Size.height - 55.f));		
		menu_author->addChild(btn_author);
		menu_author->setID("author-name");

		addChild(menu_author);

		// progress bars
		if (Level->isPlatformer()){
			auto tl = getChildByTag(10);
			auto pos = tl->getPosition();
			tl->setPosition(pos.x, pos.y - 40.f);
		} else {
			bool practice = false;
			for (int target = 7; target < 13; target ++){
				auto sprite = getChildByTag(target);
				auto pos = sprite->getPosition();
				sprite->setPosition(CCPoint(pos.x, pos.y-20.f + 5*practice));//
				practice = !practice;
			}
		}

		auto btns = getChildByTag(2);
		auto pos = btns->getPosition();
		btns->setPosition(pos.x, pos.y - 10.f);

		for (int target = 3; target < 7; target ++){
			auto sprite = getChildByTag(target);
			auto pos = sprite->getPosition();
			sprite->setPosition(CCPoint(pos.x, pos.y-5.f));// + 10*practice
			//practice = !practice;
		}

		if (Level->isPlatformer()){
			auto menu = getChildByTag(2);
			menu->removeChildByID("full-restart-button");
			menu->removeChildByID("retry-button");

			auto img_restart = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
			auto btn_restart = CCMenuItemSpriteExtra::create(img_restart, this, menu_selector(MyPauseLayer::onMultiRestart));
	
			menu->addChild(btn_restart);

			menu->setLayout(
				RowLayout::create()
					->setGap(15.f)
					->setAutoScale(true)
					->setAxisAlignment(AxisAlignment::Center)
					->setCrossAxisAlignment(AxisAlignment::Center)
			);
			menu->updateLayout();

		}

		updateLayout();
	}

	void onViewProfile(CCObject *sender) {
		auto lvl = PlayLayer::get()->m_level;
		int id = lvl->m_accountID;
		if (lvl->m_levelType == GJLevelType::Local)
			id = 71;
		ProfilePage::create(id, false)->show();
	}

	void onLevelInfo(CCObject *sender) {
		InfoLayer::create(PlayLayer::get()->m_level, nullptr, nullptr)->show();
	}
	
	void onMultiRestart(CCObject *sender){

		geode::createQuickPopup(
			"Restart",
			"Return to practice checkpoint or restart the whole level?",
			"Restart", "Full", 360,
			[this, sender](auto, bool btn2) {
				if (btn2) { PauseLayer::onRestartFull(sender); }
			 	else { PauseLayer::onRestart(sender); }
			}
		);
	}
};

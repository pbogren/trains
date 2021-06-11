/**
    @file include/ui_menu.h
    @author Patrik Bogren (pabo1800)
    @date May 2019
    @version: 0.1
    @brief Definition of the Menu class
*/
#ifndef INCLUDE_UI_MENU_H
#define INCLUDE_UI_MENU_H

#include <functional>
#include <string>
#include <utility>  // move
#include <vector>

namespace pabo::app {

using namespace std::string_literals;

class Menu {
    using Callable = std::function<void()>;

public:
    Menu(std::string title, std::string returnTxt = "Return"s);

    // Add a new menu item
    void addItem(std::string txt, Callable operation, bool isEnabled = true);
    // Disable a menu item where the text matches a pattern.
    void disableItem(const std::string& pattern);
    // Enable a menu item where the text matches a pattern.
    void enableItem(const std::string& pattern);

    // Return true if the menu is in a running state.
    [[nodiscard]] bool isRunning() const noexcept;
    // Run the menu continuously until isRunning returns false.
    void run();
    // Run the menu once.
    void runOnce();
    // Run the menu once and notify the user of errors that occurred.
    void runOnceOrPrintError();

    // Reset the state of the menu so that it can be run again.
    void reset();

private:
    struct Item;

    void show() const noexcept;
    void showTitle() const;
    void showItems() const;
    void getChoice();
    void execute();
    [[nodiscard]] std::vector<const Item*> enabledItems() const;
    [[nodiscard]] std::vector<Item*> enabledItems();

    auto findItemByPattern(const std::string& pattern);

    [[nodiscard]] bool choiceIsBad() const noexcept;

    std::string m_title;
    std::string m_retTxt;  // Text to display for the option to stop the menu.
    std::vector<Item> m_items;
    int m_choice{1};
};

struct Menu::Item {
    Item(std::string t, Menu::Callable f, bool isEnabled)
        : txt{std::move(t)}, func{std::move(f)}, m_isEnabled{isEnabled}
    {
    }

    bool isEnabled() const { return m_isEnabled; }
    void enable() { m_isEnabled = true; }
    void disable() { m_isEnabled = false; }
    bool matches(const std::string& pattern) const {
        return txt.find(pattern) != std::string::npos;
    }

    std::string txt;
    Menu::Callable func;
    bool m_isEnabled;
};

}  // namespace pabo::app

#endif

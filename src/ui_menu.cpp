#include "console_IO.h"
#include "ui_menu.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace pabo::app {

using namespace std::string_literals;

// Offset the choice to display first choice as 1 to
// user, while keeping the returning function at index 0.
constexpr int choiceOffset{1};

Menu::Menu(std::string title, std::string returnTxt)
    : m_title{std::move(title)}
    , m_retTxt{std::move(returnTxt)}
{
}

void Menu::addItem(std::string txt, Callable operation, bool isEnabled)
{
    m_items.emplace_back(std::move(txt), operation, isEnabled);
}

void Menu::run()
{
    while (isRunning()) {
        runOnceOrPrintError();
    }
    reset();
}


void Menu::runOnceOrPrintError()
{
    try {
        runOnce();
    }
    catch (const std::logic_error& e) {
        IO::println("\nError: "s + e.what());
        IO::waitForEnter();
        reset();
    }
}

void Menu::runOnce()
{
    show();
    getChoice();
    execute();
}

void Menu::reset()
{
    m_choice = 1;
}

bool Menu::isRunning() const noexcept
{
    return m_choice > 0;
}

void Menu::show() const noexcept
{
    showTitle();
    showItems();
}

void Menu::showTitle() const
{
    IO::println(m_title);
}

void Menu::showItems() const
{
    const auto enabled = enabledItems();
    for (auto i = 0U; i < enabled.size(); ++i) {
        const auto& item = enabled[i];
        IO::print(i + choiceOffset);
        IO::println(". " + item->txt);
    }
    IO::println("0. " + m_retTxt);
}

std::vector<const Menu::Item*> Menu::enabledItems() const
{
    auto res = std::vector<const Menu::Item*>{};
    for (const auto& item: m_items) {
        if (item.isEnabled()) {
            res.push_back(&item);
        }
    }
    return res;
}

std::vector<Menu::Item*> Menu::enabledItems()
{
    auto res = std::vector<Menu::Item*>{};
    for (auto& item: m_items) {
        if (item.isEnabled()) {
            res.push_back(&item);
        }
    }
    return res;
}

auto Menu::findItemByPattern(const std::string& pattern)
{
    auto it = std::find_if(m_items.begin(), m_items.end(),
                           [&pattern](const Item& item) {
                               return item.matches(pattern);
                           });
    if (it == m_items.end()) {
        throw std::invalid_argument(
                "No matching menu item in \"" + m_title + "\": " + pattern);
    }
    return it;
}


void Menu::disableItem(const std::string& pattern)
{
    auto item = findItemByPattern(pattern);
    item->disable();
}

void Menu::enableItem(const std::string& pattern)
{
    auto item = findItemByPattern(pattern);
    item->enable();
}


void Menu::getChoice()
{
    m_choice = IO::get<int>(" > ");
}

void Menu::execute()
{
    if (choiceIsBad()) {
        const auto choiceStr = std::to_string(m_choice);
        throw std::out_of_range("Invalid menu choice: " + choiceStr);
    }
    if (m_choice != 0) {
        const auto items = enabledItems();
        items[m_choice - choiceOffset]->func();
    }
}

bool Menu::choiceIsBad() const noexcept
{
    const auto items = enabledItems();
    return m_choice > static_cast<int>(items.size()) ||
           m_choice < 0;
}


//
// Non-members
//
void runMenu(Menu& menu)
{
    menu.run();
}


}  // namespace pabo::app

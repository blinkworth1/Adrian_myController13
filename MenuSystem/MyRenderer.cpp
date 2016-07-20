#include "MyRenderer.h"

MyRenderer::MyRenderer (Adafruit_SSD1306 * ptr) {
displayptr = ptr;
}

void MyRenderer::render(Menu const& menu) const
{
    displayptr->setTextColor(WHITE);
    displayptr->println(menu.get_name());
    String buffer;
    MenuComponent const* cp_menu_sel = menu.get_current_component();
    for (int i = 0; i < menu.get_num_menu_components(); ++i)
    {
        MenuComponent const* cp_m_comp = menu.get_menu_component(i);
        if (cp_menu_sel == cp_m_comp)   {
            displayptr->setTextColor(BLACK, WHITE);
        }
        else {
            displayptr->setTextColor(WHITE);
        }
        cp_m_comp->render(*this);
        displayptr->println("");
    }
}

void MyRenderer::render_menu_item(MenuItem const& menu_item) const
{
displayptr->print(menu_item.get_name());
displayptr->setTextColor(WHITE);
}

void MyRenderer::render_back_menu_item(BackMenuItem const& menu_item) const
{
displayptr->print(menu_item.get_name());
displayptr->setTextColor(WHITE);
}

void MyRenderer::render_numeric_menu_item(NumericMenuItem const& menu_item) const
{
    String buffer;

    buffer = menu_item.get_name();
    buffer += menu_item.has_focus() ? '<' : '=';

    // TODO: get_value_string is a poor name. get_formatted_value maybe?
    buffer += menu_item.get_value_string();

    if (menu_item.has_focus())
        buffer += '>';

    displayptr->print(buffer);
}

void MyRenderer::render_custom_numeric_menu_item(CustomNumericMenuItem const& menu_item) const
{
    // This condition can be put in the CustomNumericMenuItem class as well
    if (menu_item.has_focus())
    {
        // Only display the ASCII graphics in edit mode.

        String buffer;

        // make room for a ' ' at the end and the terminating 0
        char graphics[menu_item.get_width() + 2];

        // fill the string with '-'
        for (int i = 0; i < menu_item.get_width(); i++)
            graphics[i] = '-';

        // insert a '|' at the relative _value position
        graphics[int(
            (menu_item.get_width() - 1) *
                (menu_item.get_value() - menu_item.get_minValue()) /
                (menu_item.get_maxValue() - menu_item.get_minValue())
            )] = '|';
        graphics[menu_item.get_width()] = ' ';
        graphics[menu_item.get_width() + 1] = 0;
        buffer = graphics;

        buffer += menu_item.get_value();

        displayptr->println(buffer);
    }
    else
    {
        // Non edit mode: Let parent class handle this
        return render_numeric_menu_item(menu_item);
    }
}

void MyRenderer::render_menu(Menu const& menu) const
{
    
displayptr->print(menu.get_name());
displayptr->setTextColor(WHITE);
}

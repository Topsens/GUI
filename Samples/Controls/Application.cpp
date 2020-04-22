#include "Application.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "ProgressBar.h"
#include "resource.h"

using namespace std;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    Application app(hInstance);
    if (!app.Create(nullptr))
    {
        return -1;
    }

    return app.Modal();
}

Application::Application(HINSTANCE instance) : Dialog(IDD_MAIN, instance)
{
}

bool Application::OnCreated()
{
    auto combo = (ComboBox&)this->Item(IDC_COMBO);
    combo.Add(L"Hello");
    combo.Add(L"World");

    auto progress = (ProgressBar&)this->Item(IDC_PROGRESS);
    progress.SetRange(-100, 100);
    progress.Position(-50);

    return Dialog::OnCreated();
}

void Application::OnCommand()
{
    auto echo = this->Item(IDC_ECHO);

    switch (this->command)
    {
        case IDC_COMBO:
        {
            if (CBN_SELCHANGE == HIWORD(this->wparam))
            {
                auto combo = (ComboBox&)this->Item(IDC_COMBO);
                wstring text;

                if (combo.GetText(combo.Selection(), text))
                {
                    echo.Text(text);
                }
            }

            break;
        }

        case IDC_CHECK:
        {
            if (BN_CLICKED == HIWORD(this->wparam))
            {
                auto check = (CheckBox&)this->Item(IDC_CHECK);

                if (check.IsChecked())
                {
                    echo.Text(L"Checked");
                }
                else if (check.IsUnchecked())
                {
                    echo.Text(L"Unchecked");
                }
                else if (check.IsIndeterminate())
                {
                    echo.Text(L"Indeterminate");
                }
            }

            break;
        }

        case IDC_BUTTON:
        {
            echo.Text(L"Button clicked");
            break;
        }

        default:
            break;
    }
}
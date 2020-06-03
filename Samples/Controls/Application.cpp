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
    if (!Dialog::OnCreated())
    {
        return false;
    }

    auto combo = (ComboBox&)this->Item(IDC_COMBO);
    combo.Add(L"Hello");
    combo.Add(L"World");

    auto progress = (ProgressBar&)this->Item(IDC_PROGRESS);
    progress.SetRange(-100, 100);
    progress.Position(-50);

    this->RegisterCommand(IDC_COMBO, [this]
    {
        if (CBN_SELCHANGE == HIWORD(this->wparam))
        {
            auto combo = (ComboBox&)this->Item(IDC_COMBO);
            wstring text;

            if (combo.GetText(combo.Selection(), text))
            {
                this->Item(IDC_ECHO).Text(text);
            }
        }
    });

    this->RegisterCommand(IDC_CHECK, [this]
    {
        if (BN_CLICKED == HIWORD(this->wparam))
        {
            auto echo  = this->Item(IDC_ECHO);
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
    });

    this->RegisterCommand(IDC_BUTTON, [this]
    {
        this->Item(IDC_ECHO).Text(L"Button clicked");
    });
    return true;
}
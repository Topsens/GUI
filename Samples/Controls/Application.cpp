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

Application::Application(HINSTANCE instance) : Dialog(IDD_MAIN, instance), status(IDC_STATUS), font(DialogItem::CreateFont(L"MS Shell Dlg", 25))
{
}

Application::~Application()
{
    DialogItem::DestroyFont(this->font);
}

bool Application::OnCreated()
{
    if (!Dialog::OnCreated())
    {
        return false;
    }

    if (!this->status.Create(this))
    {
        return false;
    }

    auto pb = ProgressBar::Create(this->status.Handle(), IDC_PBONSB);
    if (!pb)
    {
        return false;
    }
    pb.Position(50);
    pb.Show();

    int positions[] = { 100, -1 };
    this->status.SetParts(2, positions);
    this->status.ClipChildren();
    this->status.Show();

    this->Item(IDC_ECHO).Font(this->font);

    auto combo = (ComboBox&)this->Item(IDC_COMBO);
    combo.Add(L"Hello");
    combo.Add(L"World");

    pb = (ProgressBar&)this->Item(IDC_PROGRESS);
    pb.SetRange(-100, 100);
    pb.Position(-50);

    this->OnSize();

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

        this->status.Text(this->Item(IDC_ECHO).Text());
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

        this->status.Text(this->Item(IDC_ECHO).Text());
    });

    this->RegisterCommand(IDC_BUTTON, [this]
    {
        this->Item(IDC_ECHO).Text(L"Button clicked");
        this->status.Text(this->Item(IDC_ECHO).Text());
    });
    
    return true;
}

void Application::OnSize()
{
    this->status.AutoResize();

    vector<int> positions;
    this->status.GetParts(positions);

    auto pb = this->status.Item(IDC_PBONSB);
    pb.MoveTo(positions[0], 2);
    pb.Resize(this->status.Width() - pb.X(), this->status.Height() - pb.Y());
}
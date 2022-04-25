#include "Application.h"
#include "Button.h"
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

    if (!this->CreateStatus())
    {
        return false;
    }

    this->Item(IDC_ECHO).Font(this->font);

    auto combo = (ComboBox&)this->Item(IDC_COMBO);
    combo.Add(L"Hello");
    combo.Add(L"World");

    auto style = combo.Style();

    auto progress = (ProgressBar&)this->Item(IDC_PROGRESS);
    progress.Position(50);

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
        this->status.Text(L"Button clicked");
    });

    return true;
}

void Application::OnClose()
{
    Dialog::OnClose();
    this->Destroy();
}

void Application::OnSize()
{
    this->status.Adjust();

    vector<int> positions;
    this->status.GetParts(positions);

    auto bt = this->status.Item(IDC_STATUS_BUTTON);
    bt.MoveTo(positions[0], 2);
    bt.Resize(60, this->status.Height() - bt.Y());

    auto ck = this->status.Item(IDC_STATUS_CHECK);
    ck.MoveTo(bt.X() + bt.Width() + 5, 2);
    ck.Resize(100, this->status.Height() - ck.Y());

    auto cb = this->status.Item(IDC_STATUS_COMBO);
    cb.MoveTo(ck.X() + ck.Width(), 2);
    cb.Resize(50, 120);

    auto pb = this->status.Item(IDC_STATUS_PROGRESS);
    pb.MoveTo(cb.X() + cb.Width(), 2);
    pb.Resize(this->status.Width() - cb.X() - cb.Width(), this->status.Height() - pb.Y());
}

bool Application::CreateStatus()
{
    if (!this->status.Create(this))
    {
        return false;
    }

    auto button = Button::Create(this->status.Handle(), IDC_STATUS_BUTTON, L"S-Button");
    if (!button)
    {
        return false;
    }

    auto combo = ComboBox::Create(this->status.Handle(), IDC_STATUS_COMBO);
    if (!combo)
    {
        return false;
    }

    auto check = CheckBox::Create(this->status.Handle(), IDC_STATUS_CHECK, L"Unchecked");
    if (!check)
    {
        return false;
    }

    auto progress = ProgressBar::Create(this->status.Handle(), IDC_STATUS_PROGRESS);
    if (!progress)
    {
        return false;
    }

    button.Show();

    combo.Add(L"0");
    combo.Add(L"25");
    combo.Add(L"50");
    combo.Add(L"75");
    combo.Add(L"100");
    combo.Select(2);
    combo.Show();

    check.Show();

    progress.Position(50);
    progress.Show();

    int positions[] = { 100, -1 };
    this->status.SetParts(2, positions);
    this->status.ClipChildren();
    this->status.Show();

    this->RegisterCommand(IDC_STATUS_BUTTON, [this]
    {
        this->Item(IDC_ECHO).Text(L"S-Button clicked");
        this->status.Text(L"S-Button clicked");
    });
    this->RegisterCommand(IDC_STATUS_COMBO, [this]
    {
        auto text = this->status.Item(IDC_STATUS_COMBO).Text();
        this->Item(IDC_ECHO).Text(text);
        this->status.Text(text);
        ((ProgressBar&)this->Item(IDC_PROGRESS)).Position(_wtoi(text.c_str()));
        ((ProgressBar&)this->status.Item(IDC_STATUS_PROGRESS)).Position(_wtoi(text.c_str()));
    });
    this->RegisterCommand(IDC_STATUS_CHECK, [this]
    {
        auto check = (CheckBox&)this->status.Item(IDC_STATUS_CHECK);
        check.Text(check.IsChecked() ? L"Checked" : L"Unchecked");
        this->Item(IDC_ECHO).Text(check.Text());
        this->status.Text(check.Text());
    });

    return true;
}
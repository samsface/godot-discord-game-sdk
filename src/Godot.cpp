#include <Godot.hpp>
#include <Node.hpp>
#include <cstdint>
#include <discord.h>
#include <memory>
#include <set>

namespace godot 
{
class DiscordResult : public Reference
{
    GODOT_CLASS(DiscordResult, Reference)

public:
    static void _register_methods()
    {
        register_signal<DiscordResult>("result", "result", GODOT_VARIANT_TYPE_INT);
    }

    void _init()
    {
    }
};

struct result_callback
{
    Ref<DiscordResult> data;

    result_callback() : data{DiscordResult::_new()}
    {
    }

    void operator()(discord::Result result)
    {
        data->emit_signal("result", static_cast<int>(result));
    }
};

class DiscordActivityAssets : public Reference
{
    GODOT_CLASS(DiscordActivityAssets, Reference)

    discord::ActivityAssets* data_{};

public:
    auto const& data()
    {
        return data_;
    }

    void set_large_image(String value)
    {
        if(data_)
        {
            data_->SetLargeImage(value.utf8().get_data());
        }
    }

    void set_large_text(String value)
    {
        if(data_)
        {
            data_->SetLargeText(value.utf8().get_data());
        }
    }

    void set_small_image(String value)
    {
        if(data_)
        {
            data_->SetSmallImage(value.utf8().get_data());
        }
    }

    void set_small_text(String value)
    {
        if(data_)
        {
            data_->SetSmallText(value.utf8().get_data());
        }
    }

    static auto bind(discord::ActivityAssets& activity_assets)
    {
        Ref<DiscordActivityAssets> res{DiscordActivityAssets::_new()};
        res->data_ = &activity_assets;

        return res;
    }

    static void _register_methods()
    {
        register_method("set_large_image", &DiscordActivityAssets::set_large_image);
        register_method("set_large_text", &DiscordActivityAssets::set_large_text);
        register_method("set_small_image", &DiscordActivityAssets::set_small_image);
        register_method("set_small_text", &DiscordActivityAssets::set_small_text);
    }

    void _init()
    {
    }
};

class DiscordActivitySecrets : public Reference
{
    GODOT_CLASS(DiscordActivitySecrets, Reference)

    discord::ActivitySecrets* data_{};

public:
    auto const& data()
    {
        return data_;
    }

    void set_match(String value)
    {
        if(data_)
        {
            data_->SetMatch(value.utf8().get_data());
        }
    }

    void set_join(String value)
    {
        if(data_)
        {
            data_->SetJoin(value.utf8().get_data());
        }
    }

    void set_spectate(String value)
    {
        if(data_)
        {
            data_->SetSpectate(value.utf8().get_data());
        }
    }

    static auto bind(discord::ActivitySecrets& activity_secrets)
    {
        Ref<DiscordActivitySecrets> res{DiscordActivitySecrets::_new()};
        res->data_ = &activity_secrets;

        return res;
    }

    static void _register_methods()
    {
        register_method("set_match", &DiscordActivitySecrets::set_match);
        register_method("set_join", &DiscordActivitySecrets::set_join);
        register_method("set_spectate", &DiscordActivitySecrets::set_spectate);
    }

    void _init()
    {
    }
};

class DiscordActivity : public Reference
{
    GODOT_CLASS(DiscordActivity, Reference)

    discord::Activity data_;

public:
    auto const& data()
    {
        return data_;
    }

    void set_type(int type)
    {
        data_.SetType(static_cast<discord::ActivityType>(type));
    }

    void set_application_id(std::int64_t application_id)
    {
        data_.SetApplicationId(application_id);
    }

    void set_name(String value)
    {
        data_.SetName(value.utf8().get_data());
    }

    void set_state(String value)
    {
        data_.SetState(value.utf8().get_data());
    }

    void set_details(String value)
    {
        data_.SetDetails(value.utf8().get_data());
    }

    void set_instance(bool value)
    {
        data_.SetInstance(value);
    }

    auto get_assets()
    {
        return DiscordActivityAssets::bind(data_.GetAssets());
    }

    auto get_secrets()
    {
        return DiscordActivitySecrets::bind(data_.GetSecrets());
    }

    static void _register_methods()
    {
        register_method("set_type", &DiscordActivity::set_type);
        register_method("set_application_id", &DiscordActivity::set_application_id);
        register_method("set_name", &DiscordActivity::set_name);
        register_method("set_state", &DiscordActivity::set_state);
        register_method("set_details", &DiscordActivity::set_details);
        register_method("set_instance", &DiscordActivity::set_instance);
        register_method("get_assets", &DiscordActivity::get_assets);
        register_method("get_secrets", &DiscordActivity::get_secrets);
    }

    void _init()
    {
    }
};

class DiscordActivityManager : public Reference
{
    GODOT_CLASS(DiscordActivityManager, Reference)

    discord::ActivityManager* data_{};

public:
    int register_command(String value)
    {
        if(data_)
        {
            return static_cast<int>(data_->RegisterCommand(value.utf8().get_data()));
        }

        return 1;
    }

    int register_steam(int steam_app_id)
    {
        if(data_)
        {
            return static_cast<int>(data_->RegisterSteam(steam_app_id));
        }

        return 1;
    }

    auto update_activity(Ref<DiscordActivity> activity)
    {
        result_callback callback;

        if(data_)
        {
            data_->UpdateActivity(activity->data(), callback);
        }

        return callback.data;
    }

    auto clear_activity()
    {
        result_callback callback;

        if(data_)
        {
            data_->ClearActivity(callback);
        }

        return callback.data;
    }

    static Ref<DiscordActivityManager> make(discord::ActivityManager& activity_manager)
    {
        Ref<DiscordActivityManager> res{DiscordActivityManager::_new()};
        res->data_ = &activity_manager;

        return res;
    }

    static void _register_methods()
    {
        register_method("register_command", &DiscordActivityManager::register_command);
        register_method("register_steam", &DiscordActivityManager::register_steam);
        register_method("update_activity", &DiscordActivityManager::update_activity);
        register_method("clear_activity", &DiscordActivityManager::clear_activity);
    }

    void _init()
    {
    }
};

class DiscordOverlayManager : public Reference
{
    GODOT_CLASS(DiscordOverlayManager, Reference)

    discord::OverlayManager* data_{};

public:
    auto set_locked(bool value)
    {
        result_callback callback;

        if(data_)
        {
            data_->SetLocked(value, callback);
        }

        return callback.data;
    }

    auto is_enabled()
    {
        bool res{};
    
        if(data_)
        {
            data_->IsEnabled(&res); 
        }

        return res;
    }

    auto open_activity_invite(int type)
    {
        result_callback callback;
    
        if(data_)
        {
            data_->OpenActivityInvite(static_cast<discord::ActivityActionType>(type), callback); 
        }

        return callback.data;
    }

    static Ref<DiscordOverlayManager> make(discord::OverlayManager& data)
    {
        Ref<DiscordOverlayManager> res{DiscordOverlayManager::_new()};
        res->data_ = &data;

        return res;
    }

    static void _register_methods()
    {
        register_method("set_locked", &DiscordOverlayManager::set_locked);
        register_method("is_enabled", &DiscordOverlayManager::is_enabled);
        register_method("open_activity_invite", &DiscordOverlayManager::open_activity_invite);
    }

    void _init()
    {
    }
};

class DiscordCore : public Reference
{
    GODOT_CLASS(DiscordCore, Reference)

    std::unique_ptr<discord::Core> core_;

public:
    static void _register_methods()
    {
        register_method("create", &DiscordCore::create);
        register_method("run_callbacks", &DiscordCore::run_callbacks);
        register_method("get_activity_manager", &DiscordCore::get_activity_manager);
        register_method("get_overlay_manager", &DiscordCore::get_overlay_mangager);
    }

    void _init()
    {
    }

    void create(std::int64_t application_id)
    {
        discord::Core* core{};
        discord::Result res = discord::Core::Create(application_id, DiscordCreateFlags_Default, &core);
        core_.reset(core);
    }

    void run_callbacks()
    {
        if(core_)
        {
            core_->RunCallbacks();
        }
    }

    Ref<DiscordActivityManager> get_activity_manager()
    {
        if(core_)
        {
            return DiscordActivityManager::make(core_->ActivityManager());
        }

        return {};
    }


    Ref<DiscordOverlayManager> get_overlay_mangager()
    {
        if(core_)
        {
            return DiscordOverlayManager::make(core_->OverlayManager());
        }

        return {};
    }
};

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) 
{
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) 
{
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) 
{
    godot::Godot::nativescript_init(handle);
    godot::register_class<godot::DiscordResult>();
    godot::register_class<godot::DiscordActivityAssets>();
    godot::register_class<godot::DiscordActivitySecrets>();
    godot::register_class<godot::DiscordActivity>();
    godot::register_class<godot::DiscordActivityManager>();
    godot::register_class<godot::DiscordOverlayManager>();
    godot::register_class<godot::DiscordCore>();
}
}

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
    int result_{};
    Variant data_{};

    static void _register_methods()
    {
        register_property("result", &DiscordResult::result_, {});
        register_property("data", &DiscordResult::data_, {});
        register_signal<DiscordResult>("result", "result", GODOT_VARIANT_TYPE_OBJECT);
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

    template<typename ...Args>
    void operator()(discord::Result result, Args...)
    {
        data->result_ = static_cast<int>(result);
        data->emit_signal("result", data);
    }
};

template<typename GT, typename DT>
class discord_to_godot_t : public Reference
{
public:
    using discord_type = DT;
    using godot_type = GT;

protected:
    discord_type* data_{};

public:

    auto const& data()
    {
        return data_;
    }

    template<typename op, typename ...Args>
    auto call(op o, Args ...args)
    {
        using x = decltype(std::invoke(o, data_, std::forward<Args>(args)...));

        if constexpr(std::is_same<x, discord::Result>::value)
        {
            auto res{discord::Result::InternalError};

            if(data_)
            {
                res = std::invoke(o, data_, std::forward<Args>(args)...);
            }

            return static_cast<int>(res);
        }
        else if constexpr(std::is_same<x, void>::value)
        {
           std::invoke(o, data_, std::forward<Args>(args)...);
        }
        else
        {
            if(data_)
            {
                return std::invoke(o, data_, std::forward<Args>(args)...);
            }
            else
            {
                return x{};
            }
        }
    }

    template<typename op, typename ...Args>
    auto callback(op o, Args ...args)
    {
        result_callback callback;

        if(data_)
        {
            std::invoke(o, data_, std::forward<Args>(args)..., callback);
        }

        return callback.data;
    }

    static auto bind(discord_type& type)
    {
        Ref<godot_type> res{godot_type::_new()};
        res->data_ = &type;
        res->init();

        return res;
    }

    void _init()
    {
    }

    void init()
    {
    }
};

class DiscordActivityAssets : public discord_to_godot_t<DiscordActivityAssets, discord::ActivityAssets>
{
    GODOT_CLASS(DiscordActivityAssets, Reference)

public:
    void set_large_image(String value)
    {
        call(&discord_type::SetLargeImage, value.utf8().get_data());
    }

    void set_large_text(String value)
    {
        call(&discord_type::SetLargeText, value.utf8().get_data());
    }

    void set_small_image(String value)
    {
        call(&discord_type::SetSmallImage, value.utf8().get_data());
    }

    void set_small_text(String value)
    {
        call(&discord_type::SetSmallText, value.utf8().get_data());
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

class DiscordActivitySecrets : public discord_to_godot_t<DiscordActivitySecrets, discord::ActivitySecrets>
{
    GODOT_CLASS(DiscordActivitySecrets, Reference)

public:
    void set_match(String value)
    {
        call(&discord_type::SetMatch, value.utf8().get_data());
    }

    void set_join(String value)
    {
        call(&discord_type::SetJoin, value.utf8().get_data());
    }

    void set_spectate(String value)
    {
        call(&discord_type::SetSpectate, value.utf8().get_data());
    }

    static void _register_methods()
    {
        register_method("set_match", &DiscordActivitySecrets::set_match);
        register_method("set_join", &DiscordActivitySecrets::set_join);
        register_method("set_spectate", &DiscordActivitySecrets::set_spectate);
    }
};

class DiscordPartySize : public discord_to_godot_t<DiscordPartySize, discord::PartySize>
{
    GODOT_CLASS(DiscordPartySize, Reference)

public:
    void set_current_size(int value)
    {
        call(&discord_type::SetCurrentSize, value);
    }

    auto set_max_size(int value)
    {
        return call(&discord_type::SetMaxSize, value);
    }

    static void _register_methods()
    {
        register_method("set_current_size", &DiscordPartySize::set_current_size);
        register_method("set_max_size", &DiscordPartySize::set_max_size);
    }
};

class DiscordActivityParty : public discord_to_godot_t<DiscordActivityParty, discord::ActivityParty>
{
    GODOT_CLASS(DiscordActivityParty, Reference)

public:
    void set_id(String value)
    {
        call(&discord_type::SetId, value.utf8().get_data());
    }

    auto get_size()
    {
        return DiscordPartySize::bind(data_->GetSize()); // what if is null?
    }

    static void _register_methods()
    {
        register_method("set_id", &DiscordActivityParty::set_id);
        register_method("get_size", &DiscordActivityParty::get_size);
    }
};

class DiscordActivity : public Reference
{
    GODOT_CLASS(DiscordActivity, Reference)

    discord::Activity data_;

public:
    auto const& data() const
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

    auto get_party()
    {
        return DiscordActivityParty::bind(data_.GetParty());
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
        register_method("get_party", &DiscordActivity::get_party);
    }

    void _init()
    {
    }
};

class DiscordActivityManager : public discord_to_godot_t<DiscordActivityManager, discord::ActivityManager>
{
    GODOT_CLASS(DiscordActivityManager, Reference)

public:
    auto register_command(String value)
    {
        return call(&discord_type::RegisterCommand, value.utf8().get_data());
    }

    auto register_steam(int steam_app_id)
    {
        return call(&discord_type::RegisterSteam, steam_app_id);
    }

    auto update_activity(Ref<DiscordActivity> activity)
    {
        return callback(&discord_type::UpdateActivity, activity->data());
    }

    auto clear_activity()
    {
        return callback(&discord_type::ClearActivity);  
    }

    static void _register_methods()
    {
        register_method("register_command", &DiscordActivityManager::register_command);
        register_method("register_steam", &DiscordActivityManager::register_steam);
        register_method("update_activity", &DiscordActivityManager::update_activity);
        register_method("clear_activity", &DiscordActivityManager::clear_activity);
        register_signal<DiscordActivityManager>("activity_join", "secret", GODOT_VARIANT_TYPE_OBJECT);
        register_signal<DiscordActivityManager>("activity_invite");
        register_signal<DiscordActivityManager>("activity_join_request");
    }

    void init()
    {
        data_->OnActivityJoin.Connect([&](char const* secret)
        {
            emit_signal("activity_join", String(secret));
        });

        data_->OnActivityInvite.Connect([&](auto activation_type, auto const& user, auto const& activity)
        {
           emit_signal("activity_invite");
        });

        data_->OnActivityJoinRequest.Connect([&](auto)
        {
           emit_signal("activity_join_request");
        });
    }
};

class DiscordOverlayManager : public discord_to_godot_t<DiscordOverlayManager, discord::OverlayManager>
{
    GODOT_CLASS(DiscordOverlayManager, Reference)

public:
    auto set_locked(bool value)
    {
        return callback(&discord_type::SetLocked, value);
    }

    auto is_enabled()
    {
        bool res{};
        call(&discord_type::IsEnabled, &res);
        return res;
    }

    auto open_activity_invite(int type)
    {
        return callback(&discord_type::OpenActivityInvite, static_cast<discord::ActivityActionType>(type));
    }

    static void _register_methods()
    {
        register_method("set_locked", &DiscordOverlayManager::set_locked);
        register_method("is_enabled", &DiscordOverlayManager::is_enabled);
        register_method("open_activity_invite", &DiscordOverlayManager::open_activity_invite);
    }
};

class DiscordLobbyTransaction : public Reference
{
    GODOT_CLASS(DiscordLobbyTransaction, Reference)

    discord::LobbyTransaction data_;

public:
    auto& data()
    {
        return data_;
    }

    auto set_type(int value)
    {
        return static_cast<int>(data_.SetType(static_cast<discord::LobbyType>(value)));
    }

    auto set_owner(std::int64_t value)
    {
        return static_cast<int>(data_.SetOwner(value));
    }

    auto set_capacity(std::uint32_t value)
    {
        return static_cast<int>(data_.SetCapacity(value));
    }

    auto set_metadata(String key, String value)
    {
        return static_cast<int>(data_.SetMetadata(key.utf8().get_data(), value.utf8().get_data()));
    }

    auto delete_metadata(String key)
    {
        return static_cast<int>(data_.DeleteMetadata(key.utf8().get_data()));
    }

    auto set_locked(bool value)
    {
        return static_cast<int>(data_.SetLocked(value));
    }

    static void _register_methods()
    {
        register_method("set_type", &DiscordLobbyTransaction::set_type);
        register_method("set_owner", &DiscordLobbyTransaction::set_owner);
        register_method("set_capacity", &DiscordLobbyTransaction::set_capacity);
        register_method("set_metadata", &DiscordLobbyTransaction::set_metadata);
        register_method("delete_metadata", &DiscordLobbyTransaction::delete_metadata);
        register_method("set_locked", &DiscordLobbyTransaction::set_locked);
    }

    void _init()
    {
    }
};

class DiscordLobby : public discord_to_godot_t<DiscordLobby, discord::Lobby>
{
    GODOT_CLASS(DiscordLobby, Reference)

public:
    auto get_id()
    {
        return call(&discord_type::GetId);
    }

    auto get_type()
    {
        return static_cast<int>(call(&discord_type::GetType));
    }

    auto get_owner_id()
    {
        return call(&discord_type::GetOwnerId);
    }

    auto get_secret()
    {
        return String(call(&discord_type::GetSecret));
    }

    auto get_capacity()
    {
        return call(&discord_type::GetCapacity);
    }

    auto get_locked()
    {
        return call(&discord_type::GetLocked);
    }

    static void _register_methods()
    {
        register_method("get_id", &DiscordLobby::get_id);
        register_method("get_type", &DiscordLobby::get_type);
        register_method("get_owner_id", &DiscordLobby::get_owner_id);
        register_method("get_secret", &DiscordLobby::get_secret);
        register_method("get_capacity", &DiscordLobby::get_capacity);
        register_method("get_locked", &DiscordLobby::get_locked);
    }
};

/*
    Result GetLobbyActivitySecret(LobbyId lobbyId, char secret[128]);
    Result GetLobbyMetadataValue(LobbyId lobbyId, MetadataKey key, char value[4096]);
    Result GetLobbyMetadataKey(LobbyId lobbyId, std::int32_t index, char key[256]);
    Result LobbyMetadataCount(LobbyId lobbyId, std::int32_t* count);
    Result MemberCount(LobbyId lobbyId, std::int32_t* count);
    Result GetMemberUserId(LobbyId lobbyId, std::int32_t index, UserId* userId);
    Result GetMemberUser(LobbyId lobbyId, UserId userId, User* user);
    Result GetMemberMetadataValue(LobbyId lobbyId,
                                  UserId userId,
                                  MetadataKey key,
                                  char value[4096]);
    Result GetMemberMetadataKey(LobbyId lobbyId, UserId userId, std::int32_t index, char key[256]);
    Result MemberMetadataCount(LobbyId lobbyId, UserId userId, std::int32_t* count);
    void UpdateMember(LobbyId lobbyId,
                      UserId userId,
                      LobbyMemberTransaction const& transaction,
                      std::function<void(Result)> callback);

    Result GetSearchQuery(LobbySearchQuery* query);
    void Search(LobbySearchQuery const& query, std::function<void(Result)> callback);
    void LobbyCount(std::int32_t* count);
    Result GetLobbyId(std::int32_t index, LobbyId* lobbyId);
    void ConnectVoice(LobbyId lobbyId, std::function<void(Result)> callback);
    void DisconnectVoice(LobbyId lobbyId, std::function<void(Result)> callback);
    Result ConnectNetwork(LobbyId lobbyId);
    Result DisconnectNetwork(LobbyId lobbyId);
    Result FlushNetwork();
    Result OpenNetworkChannel(LobbyId lobbyId, std::uint8_t channelId, bool reliable);
    Result SendNetworkMessage(LobbyId lobbyId,
                              UserId userId,
                              std::uint8_t channelId,
                              std::uint8_t* data,
                              std::uint32_t dataLength);

    Event<std::int64_t> OnLobbyUpdate;
    Event<std::int64_t, std::uint32_t> OnLobbyDelete;
    Event<std::int64_t, std::int64_t> OnMemberConnect;
    Event<std::int64_t, std::int64_t> OnMemberUpdate;
    Event<std::int64_t, std::int64_t> OnMemberDisconnect;
    Event<std::int64_t, std::int64_t, std::uint8_t*, std::uint32_t> OnLobbyMessage;
    Event<std::int64_t, std::int64_t, bool> OnSpeaking;
    Event<std::int64_t, std::int64_t, std::uint8_t, std::uint8_t*, std::uint32_t> OnNetworkMessage;
*/
class DiscordLobbyManager : public discord_to_godot_t<DiscordLobbyManager, discord::LobbyManager>
{
    GODOT_CLASS(DiscordLobbyManager, Reference)

public:
    auto get_lobby_create_transaction(Ref<DiscordLobbyTransaction> transaction)
    {
        return call(&discord_type::GetLobbyCreateTransaction, &transaction->data());
    }

    auto get_lobby_update_transaction(std::int64_t lobby_id, Ref<DiscordLobbyTransaction> transaction)
    {
       return call(&discord_type::GetLobbyUpdateTransaction, lobby_id, &transaction->data());
    }

    auto get_member_update_transaction(std::int64_t lobby_id, std::int64_t user_id, Ref<DiscordLobbyTransaction> transaction)
    {
       //return call(&discord_type::GetMemberUpdateTransaction, lobby_id, user_id, &transaction->data()); need to implement membertransaction
    }

    auto create_lobby(Ref<DiscordLobbyTransaction> transaction)
    {
        result_callback callback;

        if(data_)
        {
            data_->CreateLobby(transaction->data(), [callback](auto res, auto const& lobby) mutable
            {
                callback.data->result_ = static_cast<int>(res);
                callback.data->data_ =  DiscordLobby::bind(const_cast<discord::Lobby&>(lobby));
                callback.data->emit_signal("result", callback.data);
            });
        }

        return callback.data;
    }

    auto update_lobby(std::int64_t lobby_id, Ref<DiscordLobbyTransaction> transaction)
    {
        return callback(&discord_type::UpdateLobby, lobby_id, transaction->data());
    }

    auto delete_lobby(std::int64_t lobby_id)
    {
        return callback(&discord_type::DeleteLobby, lobby_id);
    }

    auto connect_lobby(std::int64_t lobby_id, String lobby_secret)
    {
        std::cout << lobby_id << std::endl;
        return callback(&discord_type::ConnectLobby, lobby_id, lobby_secret.utf8().get_data());
    }

    auto connect_lobby_with_activity_secret(String activitySecret)
    {
        result_callback callback;

        if(data_)
        {
            data_->ConnectLobbyWithActivitySecret(activitySecret.utf8().get_data(), [callback](auto res, auto const& lobby) mutable
            {
                callback.data->result_ = static_cast<int>(res);
                callback.data->data_ =  DiscordLobby::bind(const_cast<discord::Lobby&>(lobby));
                callback.data->emit_signal("result", callback.data);
            });
        }

        return callback.data;
    }

    auto disconnect_lobby(std::int64_t lobby_id)
    {
        return callback(&discord_type::DisconnectLobby, lobby_id);
    }

    auto get_lobby(std::int64_t lobby_id, Ref<DiscordLobby> lobby)
    {
        return call(&discord_type::GetLobby, lobby_id, lobby->data());
    }

    auto send_lobby_message(std::int64_t lobby_id, PoolByteArray bytes)
    {
        return callback(&discord_type::SendLobbyMessage, lobby_id, bytes.write().ptr(), bytes.size());
    }

    auto get_lobby_activity_secret(std::int64_t lobby_id)
    {
        std::array<char, 129> buffer{};
        call(&discord_type::GetLobbyActivitySecret, lobby_id, buffer.data());
        return String(buffer.data());
    }

    static void _register_methods()
    {
        register_method("get_lobby_create_transaction", &DiscordLobbyManager::get_lobby_create_transaction);
        register_method("get_lobby_update_transaction", &DiscordLobbyManager::get_lobby_update_transaction);
        register_method("get_member_update_transaction", &DiscordLobbyManager::get_member_update_transaction);
        register_method("create_lobby", &DiscordLobbyManager::create_lobby);
        register_method("update_lobby", &DiscordLobbyManager::update_lobby);
        register_method("delete_lobby", &DiscordLobbyManager::delete_lobby);
        register_method("connect_lobby", &DiscordLobbyManager::connect_lobby);
        register_method("disconnect_lobby", &DiscordLobbyManager::disconnect_lobby);
        register_method("get_lobby", &DiscordLobbyManager::get_lobby);
        register_method("send_lobby_message", &DiscordLobbyManager::send_lobby_message);
        register_method("get_lobby_activity_secret", &DiscordLobbyManager::get_lobby_activity_secret);
        register_method("connect_lobby_with_activity_secret", &DiscordLobbyManager::connect_lobby_with_activity_secret);
        register_signal<DiscordLobbyManager>("lobby_message", "lobby_id", GODOT_VARIANT_TYPE_OBJECT, "user_id", GODOT_VARIANT_TYPE_OBJECT, "message", GODOT_VARIANT_TYPE_OBJECT);
    }

    void init()
    {
        data_->OnLobbyMessage.Connect([&](auto lobby_id, auto user_id, std::uint8_t* buffer, std::uint32_t buffer_size)
        {
            String message{std::string(reinterpret_cast<const char*>(buffer), buffer_size).data()};
            emit_signal("lobby_message", lobby_id, user_id, message);
        });
    }
};

class DiscordCore : public Reference
{
    GODOT_CLASS(DiscordCore, Reference)

    std::unique_ptr<discord::Core> data_;

public:
    void create(std::int64_t application_id)
    {
        discord::Core* core{};
        discord::Result res = discord::Core::Create(application_id, DiscordCreateFlags_Default, &core);
        data_.reset(core);

        data_->SetLogHook(discord::LogLevel::Info, [&](auto level, char const* message)
        {
            emit_signal("log", String(message));
        });
    }

    void run_callbacks()
    {
        if(data_)
        {
            data_->RunCallbacks();
        }
    }

    template<typename manager_type, typename get_manager_op>
    auto get_manager(get_manager_op op)
    {
        if(data_)
        {
            return manager_type::bind(std::invoke(op, data_));
        }

        return Ref<manager_type>{};
    }

    auto get_activity_manager()
    {
        return get_manager<DiscordActivityManager>(&discord::Core::ActivityManager);
    }

    auto get_overlay_mangager()
    {
        return get_manager<DiscordOverlayManager>(&discord::Core::OverlayManager);
    }

    auto get_lobby_mangager()
    {
        return get_manager<DiscordLobbyManager>(&discord::Core::LobbyManager);
    }

    static void _register_methods()
    {
        register_method("create", &DiscordCore::create);
        register_method("run_callbacks", &DiscordCore::run_callbacks);
        register_method("get_activity_manager", &DiscordCore::get_activity_manager);
        register_method("get_overlay_manager", &DiscordCore::get_overlay_mangager);
        register_method("get_lobby_manager", &DiscordCore::get_lobby_mangager);
        register_signal<DiscordCore>("log", "message", GODOT_VARIANT_TYPE_OBJECT);
    }

    void _init()
    {
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
    godot::register_class<godot::DiscordPartySize>();
    godot::register_class<godot::DiscordActivityParty>();
    godot::register_class<godot::DiscordActivitySecrets>();
    godot::register_class<godot::DiscordActivity>();
    godot::register_class<godot::DiscordActivityManager>();
    godot::register_class<godot::DiscordOverlayManager>();
    godot::register_class<godot::DiscordLobbyTransaction>();
    godot::register_class<godot::DiscordLobby>();
    godot::register_class<godot::DiscordLobbyManager>();
    godot::register_class<godot::DiscordCore>();
}
}

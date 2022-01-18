extends Node2D

var lobby_id_
var secret_

func _ready() -> void:
	Discord.discore_core_.connect("log", self, "log_")
	Discord.activity_manager.connect("activity_join", self, "activity_join_")
	Discord.activity_manager.connect("activity_invite", self, "activity_invite_")
	Discord.activity_manager.connect("activity_join_request", self, "activity_join_request_")
	Discord.lobby_manager.connect("lobby_message", self, "lobby_message_")

func log_(message) -> void:
	$log.text += message + "\n"

func activity_join_(secret) -> void:
	prints("SECRET", secret)
	$secret.text = secret
	
	yield(get_tree(), "idle_frame")
	
	$secret.text = secret + " sam"

	var res = yield(Discord.lobby_manager.connect_lobby_with_activity_secret(secret), "result")
	
	$secret.text = str(res.result)
	if res.result == Discord.Result.Ok:
		$secret.text = "ok"
		#find_node("error").text = str(res.result)
		lobby_id_ = res.data.get_id()
		find_node("lobby_id").text = str(lobby_id_)
	else:
		$secret.text = "not ok"
		#find_node("error").text = str(res.result)

func activity_invite_() -> void:
	prints("INVITE")
	$secret.text = "invite"

func activity_join_request_() -> void:
	pass

func lobby_message_(lobby_id, user_id, message) -> void:
	find_node("text").text += message + "\n"

func _on_create_lobby_pressed() -> void:
	var lobby_manager = Discord.get_lobby_manager()
	var transaction = DiscordLobbyTransaction.new()
	lobby_manager.get_lobby_create_transaction(transaction)
	
	transaction.set_capacity(6)
	transaction.set_type(Discord.LobbyType.Private)
	transaction.set_locked(false)
	transaction.set_metadata("a", "123");

	var create_lobby_res = yield(lobby_manager.create_lobby(transaction), "result")
	if create_lobby_res.result == Discord.Result.Ok:
		var lobby = create_lobby_res.data
		lobby_id_ = lobby.get_id()
		secret_ = lobby.get_secret()
		find_node("lobby_id").text = str(lobby_id_)
		
		var activity = DiscordActivity.new()
		#activity.set_application_id(932734837922611310)
		activity.set_type(Discord.ActivityType.Playing)
		activity.set_state("Reached: 2-2")
		activity.set_name("NAME NAME NAME")
		activity.set_details("Loadout: grenade + rail gun")
		var assets = activity.get_assets()
		assets.set_large_image("zone2")
		assets.set_large_text("ZONE 2 WOOO")
		assets.set_small_image("capsule_main")
		assets.set_small_text("ZONE 2 WOOO")

		var secrets = activity.get_secrets()
		secrets.set_join(str(lobby_id_) + ":" + secret_)
		#secrets.set_match(str(lobby_id_) + ":match")
		secrets.set_spectate(str(lobby_id_) + ":foo spectateSecret")

		var party = activity.get_party()
		party.set_id(str(lobby_id_))
		party.get_size().set_current_size(1)
		party.get_size().set_max_size(6)

		var activity_manager = Discord.get_activity_manager()
		if activity_manager:
			#var res = activity_manager.register_steam(1543290)
			var res = activity_manager.register_command("/media/sam/adffc9be-fe94-4c6b-87db-cca9cb566739/work/godot-discord/app/export/game.x86_64")
			var clear = yield(activity_manager.clear_activity(), "result").result
			#var res = activity_manager.register_command("echo sam")

			var res2 = yield(activity_manager.update_activity(activity), "result")
			prints("res2", res2)

func _on_invite_pressed() -> void:
	var overlay_manager = Discord.get_overlay_manager()
	print(overlay_manager.is_enabled())
	
	var res4 = yield(overlay_manager.open_activity_invite(Discord.ActivityActionType.Join), "result")
	print(res4)


func _on_chat_text_entered(new_text:String):
	if lobby_id_:
		$chat.clear()
		var send_lobby_message_result = yield(Discord.lobby_manager.send_lobby_message(lobby_id_, new_text.to_utf8()), "result")
		print(send_lobby_message_result)

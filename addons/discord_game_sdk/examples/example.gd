extends Node2D

var lobby_id_
var secret_

var members_ := []

func _ready() -> void:
	Discord.discore_core_.connect("log", self, "log_")
	Discord.activity_manager.connect("activity_join", self, "activity_join_")
	Discord.activity_manager.connect("activity_invite", self, "activity_invite_")
	Discord.activity_manager.connect("activity_join_request", self, "activity_join_request_")
	Discord.lobby_manager.connect("lobby_message", self, "lobby_message_")
	Discord.lobby_manager.connect("member_connect", self, "member_connect_")
	
	var res = Discord.activity_manager.register_command("/media/sam/adffc9be-fe94-4c6b-87db-cca9cb566739/work/godot-discord/app/export/game.x86_64")
	if res != Discord.Result.Ok:
		push_error(res)

func log_(message) -> void:
	$log.text += message + "\n"

func activity_join_(secret:String) -> void:
	var result = yield(Discord.lobby_manager.connect_lobby_with_activity_secret(secret), "result")

	if result.result == Discord.Result.Ok:
		lobby_id_ = result.data.get_id()
	else:
		push_error(result.result)

func activity_invite_() -> void:
	prints("INVITE")
	$secret.text = "invite"

func activity_join_request_() -> void:
	pass

func lobby_message_(lobby_id:int, user_id:int, message:String) -> void:
	find_node("text").text += message + "\n"

func member_connect_(lobby_id:int, user:Discord.User) -> void:
	print(user.get_username())
	

func _on_create_lobby_pressed() -> void:
	var transaction := Discord.lobby_manager.get_lobby_create_transaction()

	transaction.set_capacity(6)
	transaction.set_type(Discord.LobbyType.Private)
	transaction.set_locked(false)

	var result = yield(Discord.lobby_manager.create_lobby(transaction), "result")
	if result.result != Discord.Result.Ok:
		push_error(result.result)
		return

	var lobby = result.data
	lobby_id_ = lobby.get_id()
	secret_ = lobby.get_secret()

	find_node("lobby_id").text = str(lobby_id_)
	
	update_activity_()

func update_activity_() -> void:
	var activity = Discord.Activity.new()
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
	var party = activity.get_party()
	party.set_id(str(lobby_id_))
	party.get_size().set_current_size(1)
	party.get_size().set_max_size(6)

	var result = yield(Discord.activity_manager.update_activity(activity), "result").result
	if result != Discord.Result.Ok:
		push_error(result)

func _on_invite_pressed() -> void:
	var result = yield(Discord.overlay_manager.open_activity_invite(Discord.ActivityActionType.Join), "result").result
	if result != Discord.Result.Ok:
		push_error(result)

func _on_chat_text_entered(new_text:String):
	if not lobby_id_:
		return

	$chat.clear()
	var result = yield(Discord.lobby_manager.send_lobby_message(lobby_id_, new_text), "result").result
	if result != Discord.Result.Ok:
		push_error(result)

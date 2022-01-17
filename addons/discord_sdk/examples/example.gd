extends Node2D

func _ready():
	var activity = DiscordActivity.new()
	activity.set_application_id(931968567249879101)
	activity.set_type(Discord.ActivityType.Playing)
	activity.set_state("Reached: 2-2")
	activity.set_name("NAME NAME NAME")
	activity.set_details("Loadout: grenade + rail gun")
	var assets = activity.get_assets()
	assets.set_large_image("zone2")
	assets.set_large_text("ZONE 2 WOOO")
	assets.set_small_image("capsule_main")
	assets.set_small_text("ZONE 2 WOOO")
	
	var lobby_manager = Discord.get_lobby_manager()
	var transaction = DiscordLobbyTransaction.new()
	lobby_manager.get_lobby_create_transaction(transaction)
	
	
	transaction.set_capacity(6)
	transaction.set_type(Discord.LobbyType.Public)
	transaction.set_metadata("a", "123");

	var create_lobby_res = yield(lobby_manager.create_lobby(transaction), "result")
	if create_lobby_res.result == Discord.Result.Ok:
		var lobby = create_lobby_res.data
		print(lobby.get_id())
	
	var activity_manager = Discord.get_activity_manager()
	if activity_manager:
		var res = activity_manager.register_steam(1435470)
		var clear = yield(activity_manager.clear_activity(), "result").result
		#var res = activity_manager.register_command("echo sam")

		var res2 = yield(activity_manager.update_activity(activity), "result")
		prints("res2", res2)
		
		var overlay_manager = Discord.get_overlay_manager()
		print(overlay_manager.is_enabled())
		
		var res4 = yield(overlay_manager.open_activity_invite(Discord.ActivityActionType.Join), "result")
		print(res4)
		
		var res3 = yield(overlay_manager.set_locked(true), "result")
		print(res3)

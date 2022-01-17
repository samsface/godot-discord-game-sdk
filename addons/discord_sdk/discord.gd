extends Node

enum Result {
	Ok = 0,
	ServiceUnavailable = 1,
	InvalidVersion = 2,
	LockFailed = 3,
	InternalError = 4,
	InvalidPayload = 5,
	InvalidCommand = 6,
	InvalidPermissions = 7,
	NotFetched = 8,
	NotFound = 9,
	Conflict = 10,
	InvalidSecret = 11,
	InvalidJoinSecret = 12,
	NoEligibleActivity = 13,
	InvalidInvite = 14,
	NotAuthenticated = 15,
	InvalidAccessToken = 16,
	ApplicationMismatch = 17,
	InvalidDataUrl = 18,
	InvalidBase64 = 19,
	NotFiltered = 20,
	LobbyFull = 21,
	InvalidLobbySecret = 22,
	InvalidFilename = 23,
	InvalidFileSize = 24,
	InvalidEntitlement = 25,
	NotInstalled = 26,
	NotRunning = 27,
	InsufficientBuffer = 28,
	PurchaseCanceled = 29,
	InvalidGuild = 30,
	InvalidEvent = 31,
	InvalidChannel = 32,
	InvalidOrigin = 33,
	RateLimited = 34,
	OAuth2Error = 35,
	SelectChannelTimeout = 36,
	GetGuildTimeout = 37,
	SelectVoiceForceRequired = 38,
	CaptureShortcutAlreadyListening = 39,
	UnauthorizedForAchievement = 40,
	InvalidGiftCode = 41,
	PurchaseError = 42,
	TransactionAborted = 43,
}

enum ActivityType {
	Playing = 0,
	Streaming = 1,
	Listening = 2,
	Watching = 3,
}

enum ActivityActionType {
	Join = 1,
	Spectate,
};

enum LobbyType {
	Private = 1,
	Public,
};

class Activity:
	var data_ := DiscordActivity.new()
	
	func set_type(value:int) -> void:
		data_.set_type(value)
	
	func set_application_id(value:int) -> void:
		data_.set_application_id(value)
	
	func set_name(value:String) -> void:
		data_.set_name(value)
	
	func set_state(value:String) -> void:
		data_.set_state(value)
	
	func set_details(value:String) -> void:
		data_.set_details(value)

var discore_core_:DiscordCore

func _ready():
	discore_core_ = DiscordCore.new()
	if discore_core_:
		discore_core_.create(931968567249879101)
	
func _process(delta:float) -> void:
	if discore_core_:
		discore_core_.run_callbacks()

func get_activity_manager() -> DiscordActivityManager:
	if discore_core_:
		return discore_core_.get_activity_manager()
	return null
	
func get_overlay_manager() -> DiscordOverlayManager:
	if discore_core_:
		return discore_core_.get_overlay_manager()
	return null

func get_lobby_manager() -> DiscordLobbyManager:
	if discore_core_:
		return discore_core_.get_lobby_manager()
	return null

extends Control

var game_state_ := GameState.new()

func _ready() -> void:
	game_state_.connect("chat_changed", self, "_on_chat_changed")
	game_state_.connect("members_changed", self, "_on_members_changed")

func _on_chat_changed() -> void:
	$chat.text = ""
	for chat_message in game_state_.chat:
		var member = game_state_.members.get(chat_message.member_id)
		if not member:
			return
		$chat.text += "\n" + member.username + ": " + chat_message.message

	$chat.scroll_vertical = 9999999

func _on_members_changed() -> void:
	$members.text = ""
	for member in game_state_.members.values():
		$members.text += member.username + "\n"

func _on_text_entered(new_text):
	$lobby.send_message(new_text)
	$line_edit.clear()

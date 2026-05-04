extends Control

func _ready():
	# Podríamos añadir animaciones aquí si lo deseamos
	var label = $CenterContainer/VBoxContainer/Label
	label.modulate.a = 0
	var tween = create_tween()
	tween.tween_property(label, "modulate:a", 1.0, 1.0).set_trans(Tween.TRANS_SINE)

func _on_boton_menu_pressed():
	get_tree().change_scene_to_file("res://scenes/menu_inicial.tscn")

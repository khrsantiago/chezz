extends Control

func _ready():
	# Animación de entrada para el menú inicial
	var container = $VBoxContainer
	container.modulate.a = 0
	var tween = create_tween()
	tween.tween_property(container, "modulate:a", 1.0, 0.8).set_trans(Tween.TRANS_SINE)

func _on_boton_tutorial_pressed():
	get_tree().change_scene_to_file("res://scenes/Turotial/leccion_01.tscn")

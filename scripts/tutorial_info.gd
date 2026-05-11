extends CanvasLayer

@export_multiline var texto_explicativo: String = "Texto de ejemplo..."

func _ready():
	$Panel/Label.text = texto_explicativo
	
	var tutorial_panel = $Panel
	
	var target_y = 500 # 600 (pantalla) - 80 (alto) - 20 (margen)
	var start_y = 650
	
	tutorial_panel.position = Vector2(100, start_y)
	
	# Animación de entrada
	var entrance_tween = create_tween()
	entrance_tween.tween_property(tutorial_panel, "position:y", target_y, 0.5).set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_OUT)

func _on_panel_gui_input(event: InputEvent) -> void:
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.pressed:
		ocultar_panel()

func ocultar_panel():
	var tutorial_panel = $Panel
	
	# Animación de salida (lo devolvemos a Y = 650)
	var exit_tween = create_tween()
	exit_tween.tween_property(tutorial_panel, "position:y", 650, 0.3).set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_IN)
	
	# Cuando termine la animación, apagamos el panel
	exit_tween.tween_callback(tutorial_panel.hide)

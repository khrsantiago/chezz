extends Area2D

signal indicador_clickeado

var color_dibujo: Color = Color.GRAY

func _ready():
	input_pickable = true
	add_to_group("indicadores_movimiento")

func _draw():
	# Dibujar un circulo
	draw_circle(Vector2.ZERO, 15, color_dibujo)

func _input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
		emit_signal("indicador_clickeado")
		get_viewport().set_input_as_handled()

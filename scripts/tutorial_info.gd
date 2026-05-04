extends CanvasLayer

@export_multiline var texto_explicativo: String = "Texto de ejemplo..."

func _ready():
	$PanelContainer/MarginContainer/Label.text = texto_explicativo
	
	# Animación de entrada
	var panel = $PanelContainer
	panel.position.y += 100
	var tween = create_tween()
	tween.tween_property(panel, "position:y", panel.position.y - 100, 0.5).set_trans(Tween.TRANS_BACK).set_ease(Tween.EASE_OUT)

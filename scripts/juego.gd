extends Node2D

@onready var tablero = $Tablero
@onready var zona_prohibida = $Tablero/ZonaProhibida
@onready var boton_ok = $CanvasLayer/BotonOK

var fase_colocacion = "blancas"
var turno = "blancas"

func _ready():
	# Activar modo arrastre para las piezas blancas al inicio
	for child in tablero.get_children():
		if child is Area2D:
			# Asignar tipos de pieza según el nombre para que funcionen las reglas
			if "Peon" in child.name:
				child.tipo = child.TipoPieza.PEON
			elif "Torre" in child.name:
				child.tipo = child.TipoPieza.TORRE
			
			if child.get("es_blanca") == true:
				child.es_arrastrable = true
	
	# La zona prohibida inicial para blancas (usamos la que ya está configurada en la escena)
	zona_prohibida.visible = true

func cambiar_turno():
	if turno == "blancas":
		turno = "negras"
	else:
		turno = "blancas"
	print("--- Turno de: ", turno, " ---")

func _on_boton_ok_pressed():
	if fase_colocacion == "blancas":
		# Terminar fase blancas
		for child in tablero.get_children():
			if child is Area2D and child.get("es_blanca") == true:
				child.es_arrastrable = false
		
		# Cambiar zona prohibida para proteger a las blancas (ahora el gris va abajo)
		zona_prohibida.offset_top = 3 * 64
		zona_prohibida.offset_bottom = 8 * 64
		zona_prohibida.z_index = 1 # Poner por encima de las piezas blancas
		
		# Activar fase negras
		fase_colocacion = "negras"
		for child in tablero.get_children():
			if child is Area2D and child.get("es_blanca") == false:
				child.es_arrastrable = true
		
		print("Fase de colocación de NEGRAS iniciada.")
		
	elif fase_colocacion == "negras":
		# Terminar todo e iniciar el juego
		for child in tablero.get_children():
			if child is Area2D:
				child.es_arrastrable = false
		
		zona_prohibida.visible = false
		fase_colocacion = "jugando"
		turno = "blancas"
		
		boton_ok.disabled = true
		boton_ok.text = "¡A Jugar!"
		print("Fase de colocación terminada. Empiezan las BLANCAS.")

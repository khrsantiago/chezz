extends Area2D

enum TipoPieza { NORMAL, ARQUERO, PORTAL, OGRO, MAGO, ESQUELETO, DRAGON }
@export var tipo: TipoPieza = TipoPieza.NORMAL

# @export permite que esta variable aparezca en el Inspector.
# PackedScene nos permite arrastrar un archivo de escena directamente ahí.
@export var siguiente_leccion: PackedScene 
@export var movimiento: Vector2 = Vector2(0, -64)
@export var es_ataque_a_distancia: bool = false # Para el Arquero o similares

@export var es_blanca: bool = true

func _ready():
	if tipo == TipoPieza.MAGO:
		add_to_group("mago")

func _on_input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
		print("Clic detectado en: ", name)
		# Para evitar que el jugador haga clic múltiples veces mientras se mueve
		input_pickable = false 
		mover_hacia_adelante()

func mover_hacia_adelante():
	var destino_global = global_position + movimiento
	var tiene_objetivo = hay_enemigo_en(destino_global)
	
	# REGLA DEL PORTAL: Intercambio
	if tipo == TipoPieza.PORTAL:
		if tiene_objetivo:
			intercambiar_con(destino_global)
		else:
			print("El Portal solo puede intercambiarse con otras piezas.")
			input_pickable = true
		return

	# REGLA DEL OGRO (Rook move, King attack)
	if tipo == TipoPieza.OGRO:
		var dist_x = abs(movimiento.x)
		var dist_y = abs(movimiento.y)
		var max_dist = max(dist_x, dist_y)
		
		if tiene_objetivo:
			if max_dist > 80: 
				print("El Ogro solo puede capturar piezas adyacentes (como un Rey)")
				input_pickable = true 
				return 
		else:
			if dist_x != 0 and dist_y != 0:
				print("El Ogro se mueve en línea recta (como una Torre)")
				input_pickable = true
				return

	# Desactivamos colisiones mientras nos movemos para no "comer" piezas en el camino
	monitoring = false
	monitorable = false

	var tween = create_tween()
	
	var es_ataque = es_ataque_a_distancia
	if tipo == TipoPieza.ARQUERO:
		var es_diagonal = abs(movimiento.x) == abs(movimiento.y)
		es_ataque = es_ataque_a_distancia or (tiene_objetivo and es_diagonal)

	if es_ataque:
		tween.tween_property(self, "scale", Vector2(1.2, 1.2), 0.1)
		tween.tween_property(self, "scale", Vector2(1.0, 1.0), 0.1)
		confirmar_captura_distancia()
	else:
		var es_salto = (abs(movimiento.x) != abs(movimiento.y) and movimiento.x != 0 and movimiento.y != 0)
		if es_salto:
			if abs(movimiento.x) > abs(movimiento.y):
				tween.tween_property(self, "position:x", position.x + movimiento.x, 0.3)
				tween.tween_property(self, "position:y", position.y + movimiento.y, 0.3)
			else:
				tween.tween_property(self, "position:y", position.y + movimiento.y, 0.3)
				tween.tween_property(self, "position:x", position.x + movimiento.x, 0.3)
		else:
			tween.tween_property(self, "position", position + movimiento, 0.5)

	tween.tween_callback(finalizar_movimiento)

func finalizar_movimiento():
	# Al llegar al destino, capturamos si hay alguien
	var victima = obtener_pieza_en(global_position)
	if victima:
		capturar_pieza(victima)
	
	monitoring = true
	monitorable = true
	procesar_fin_de_turno()

func intercambiar_con(objetivo_pos: Vector2):
	var otra_pieza = obtener_pieza_en(objetivo_pos)
	if otra_pieza:
		var mi_pos_original = position
		var su_pos_original = otra_pieza.position
		
		var tween = create_tween().set_parallel(true)
		tween.tween_property(self, "position", su_pos_original, 0.5)
		tween.tween_property(otra_pieza, "position", mi_pos_original, 0.5)
		
		await tween.finished
		procesar_fin_de_turno()

func confirmar_captura_distancia():
	var objetivo = global_position + movimiento
	var victima = obtener_pieza_en(objetivo)
	if victima:
		capturar_pieza(victima)

func obtener_pieza_en(pos: Vector2) -> Node2D:
	var espacio = get_world_2d().direct_space_state
	var query = PhysicsPointQueryParameters2D.new()
	query.position = pos
	query.collide_with_areas = true
	var resultados = espacio.intersect_point(query)
	for res in resultados:
		if res.collider != self:
			return res.collider
	return null

func capturar_pieza(victima: Node2D):
	# Verificar si hay un Mago aliado cerca para revivir
	var magos = get_tree().get_nodes_in_group("mago")
	for mago in magos:
		# Si el mago es del MISMO EQUIPO que la víctima
		if mago.es_blanca == victima.get("es_blanca") and mago.global_position.distance_to(victima.global_position) < 200:
			print("¡Pieza salvada por el Mago: ", victima.name, "!")
			victima.visible = false
			return
	
	victima.queue_free()

func procesar_fin_de_turno():
	print("--- Fin de turno ---")
	if siguiente_leccion != null:
		await get_tree().create_timer(0.5).timeout 
		get_tree().change_scene_to_packed(siguiente_leccion)
	else:
		input_pickable = true

func hay_enemigo_en(posicion_objetivo: Vector2) -> bool:
	return obtener_pieza_en(posicion_objetivo) != null

func _on_area_entered(area: Area2D) -> void:
	# Ya no capturamos aquí automáticamente para evitar capturas accidentales al movernos
	pass

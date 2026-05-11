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
		if not es_blanca:
			return
		print("Clic detectado en: ", name)
		mostrar_indicador()
		get_viewport().set_input_as_handled()

func mostrar_indicador():
	limpiar_indicadores()
	
	var destino_local = position + movimiento
	var destino_global = get_parent().to_global(destino_local)
	var tiene_objetivo = hay_enemigo_en(destino_global)
	
	# REGLA DEL PORTAL
	if tipo == TipoPieza.PORTAL and not tiene_objetivo:
		print("El Portal solo puede intercambiarse con otras piezas.")
		return
		
	# REGLA DEL OGRO
	if tipo == TipoPieza.OGRO:
		var dist_x = abs(movimiento.x)
		var dist_y = abs(movimiento.y)
		var max_dist = max(dist_x, dist_y)
		if tiene_objetivo and max_dist > 80:
			print("El Ogro solo puede capturar piezas adyacentes (como un Rey)")
			return
		if not tiene_objetivo and dist_x != 0 and dist_y != 0:
			print("El Ogro se mueve en línea recta (como una Torre)")
			return

	var indicador_instancia = Area2D.new()
	var script = load("res://scripts/indicador.gd")
	indicador_instancia.set_script(script)
	
	var shape = CollisionShape2D.new()
	var rect = RectangleShape2D.new()
	rect.size = Vector2(64, 64)
	shape.shape = rect
	indicador_instancia.add_child(shape)
	
	indicador_instancia.color_dibujo = Color.RED if tiene_objetivo else Color.GRAY
	indicador_instancia.global_position = destino_global
	indicador_instancia.z_index = 10
	
	get_tree().current_scene.add_child(indicador_instancia)
	indicador_instancia.connect("indicador_clickeado", Callable(self, "_on_indicador_clickeado"))

func _on_indicador_clickeado():
	limpiar_indicadores()
	input_pickable = false 
	mover_hacia_adelante()

func limpiar_indicadores():
	var indicadores = get_tree().get_nodes_in_group("indicadores_movimiento")
	for ind in indicadores:
		ind.queue_free()

func _unhandled_input(event):
	if event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
		limpiar_indicadores()

func mover_hacia_adelante():
	var destino_local = position + movimiento
	var destino_global = get_parent().to_global(destino_local)
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
	var objetivo_local = position + movimiento
	var objetivo = get_parent().to_global(objetivo_local)
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
		if res.collider != self and not res.collider.is_in_group("indicadores_movimiento"):
			return res.collider
	return null

func capturar_pieza(victima: Node2D):
	# Verificar si hay un Mago aliado cerca para revivir
	var magos = get_tree().get_nodes_in_group("mago")
	for mago in magos:
		# Si el mago es del MISMO EQUIPO que la víctima
		if mago.es_blanca == victima.get("es_blanca") and mago.global_position.distance_to(victima.global_position) < 200:
			print("¡Pieza salvada por el Mago: ", victima.name, "!")
			var tween = create_tween()
			tween.tween_property(victima, "position:x", -64, 0.5)
			victima.monitoring = false
			victima.monitorable = false
			victima.input_pickable = false
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

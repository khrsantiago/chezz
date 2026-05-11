extends Area2D

enum TipoPieza { NORMAL, ARQUERO, PORTAL, OGRO, MAGO, ESQUELETO, DRAGON, PEON, TORRE }
@export var tipo: TipoPieza = TipoPieza.NORMAL

@export var siguiente_leccion: PackedScene 
@export var movimiento: Vector2 = Vector2(0, -64)
@export var es_ataque_a_distancia: bool = false
@export var es_blanca: bool = true
@export var es_arrastrable: bool = false

var arrastrando: bool = false
var pos_inicial_arrastre: Vector2

func _ready():
	if tipo == TipoPieza.MAGO:
		add_to_group("mago")

func _on_input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		if event.pressed:
			if es_arrastrable:
				arrastrando = true
				pos_inicial_arrastre = position
				z_index = 100
				get_viewport().set_input_as_handled()
			else:
				var scene = get_tree().current_scene
				var fase = scene.get("fase_colocacion")
				if fase == "jugando":
					var turno = scene.get("turno")
					if (turno == "blancas" and es_blanca) or (turno == "negras" and not es_blanca):
						mostrar_indicadores_juego()
						get_viewport().set_input_as_handled()
				elif es_blanca:
					mostrar_indicador_simple()
					get_viewport().set_input_as_handled()

func mostrar_indicadores_juego():
	limpiar_indicadores()
	var movs = []
	
	if tipo == TipoPieza.PEON:
		var dir = -1 if es_blanca else 1
		var adelante = Vector2(0, dir * 64)
		if not hay_pieza_en(to_global(adelante)):
			movs.append(adelante)
		var diag1 = Vector2(64, dir * 64)
		var diag2 = Vector2(-64, dir * 64)
		if hay_enemigo_en(to_global(diag1)): movs.append(diag1)
		if hay_enemigo_en(to_global(diag2)): movs.append(diag2)
		
	elif tipo == TipoPieza.TORRE:
		var direcciones = [Vector2.UP, Vector2.DOWN, Vector2.LEFT, Vector2.RIGHT]
		for d in direcciones:
			for i in range(1, 8):
				var m = d * (i * 64)
				var pos_global = to_global(m)
				if hay_pieza_en(pos_global):
					if hay_enemigo_en(pos_global):
						movs.append(m)
					break
				movs.append(m)

	for m in movs:
		instanciar_indicador(m)

func instanciar_indicador(mov_local):
	var destino_global = to_global(mov_local)
	var tiene_objetivo = hay_enemigo_en(destino_global)
	
	var indicador_instancia = Area2D.new()
	indicador_instancia.set_script(load("res://scripts/indicador.gd"))
	
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

func _on_indicador_clickeado(pos_global):
	limpiar_indicadores()
	# Calculamos el vector de movimiento real hacia donde se hizo clic
	movimiento = get_parent().to_local(pos_global) - position
	mover_hacia_adelante()

func mostrar_indicador_simple():
	# Versión para tutoriales (un solo movimiento fijo)
	limpiar_indicadores()
	instanciar_indicador(movimiento)

func limpiar_indicadores():
	var indicadores = get_tree().get_nodes_in_group("indicadores_movimiento")
	for ind in indicadores:
		ind.queue_free()

func _input(event):
	if arrastrando and event is InputEventMouseMotion:
		position = get_parent().to_local(get_global_mouse_position())
		
	if arrastrando and event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and not event.pressed:
		arrastrando = false
		z_index = 0
		var x_grid = floor(position.x / 64) * 64 + 32
		var y_grid = floor(position.y / 64) * 64 + 32
		var fila = floor(y_grid / 64)
		var columna = floor(x_grid / 64)
		
		if es_blanca:
			if fila >= 3 and fila <= 5 and columna >= 0 and columna <= 5:
				position = Vector2(x_grid, y_grid)
			else:
				position = pos_inicial_arrastre
		else:
			if fila >= 0 and fila <= 2 and columna >= 0 and columna <= 5:
				position = Vector2(x_grid, y_grid)
			else:
				position = pos_inicial_arrastre

func mover_hacia_adelante():
	var destino_global = to_global(movimiento)
	var tiene_objetivo = hay_enemigo_en(destino_global)
	
	monitoring = false
	monitorable = false
	input_pickable = false

	var tween = create_tween()
	tween.tween_property(self, "position", position + movimiento, 0.5)
	tween.tween_callback(finalizar_movimiento)

func finalizar_movimiento():
	var victima = obtener_pieza_en(global_position)
	if victima:
		capturar_pieza(victima)
	
	monitoring = true
	monitorable = true
	procesar_fin_de_turno()

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
	victima.queue_free()

func procesar_fin_de_turno():
	if siguiente_leccion != null:
		await get_tree().create_timer(0.5).timeout 
		get_tree().change_scene_to_packed(siguiente_leccion)
	else:
		var scene = get_tree().current_scene
		if scene.has_method("cambiar_turno"):
			scene.cambiar_turno()
		input_pickable = true

func hay_pieza_en(pos: Vector2) -> bool:
	return obtener_pieza_en(pos) != null

func hay_enemigo_en(pos: Vector2) -> bool:
	var victima = obtener_pieza_en(pos)
	if victima:
		return victima.get("es_blanca") != es_blanca
	return false

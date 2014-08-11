# OPifex Model Format Saving
# OPifex Entertainment LLC

#################################################################
### Imports
#################################################################

import io_export_opm.export_opm_utilities       as OPMutil
import io_export_opm.export_opm_binary		    as OPMbinary


#################################################################
### Private Methods
#################################################################

def _writeVertices(fp, model):
	for vert in model['vertices']:
		OPMbinary.Vec3(fp, vert.co)

def _writeNormals(fp, model):
	for norm in model['normals']:
		OPMutil.Print(norm)
		OPMbinary.Vec3(fp, norm)

def _writeUVs(fp, model):
	for uv in model['uvs']:
		OPMutil.Print(uv)
		OPMbinary.Vec2(fp, uv)

def _writeColors(fp, model):
	for color in model['colors']:
		OPMutil.Print(color)
		OPMbinary.Vec3(fp, color)

def _writeFaces(fp, model, options):

	local_dict = {} # local dictionary
	local_faces_list   = [] # lcoal faces index list
	local_vertex_list  = [] # local vertex list
	local_normal_list  = [] # local normal list
	local_uv_list = [] # local uv list
	local_color_list = []
	local_vertex_count = 0 # local vertices count

	vertexes = []

	for face in model['model']['faces']:
		faceFeatures = face[0]

		isNotTriangle = OPMutil.GetBit(faceFeatures, 0)
		hasMaterial = OPMutil.GetBit(faceFeatures, 1)
		hasFaceVertexUvs = OPMutil.GetBit(faceFeatures, 3)
		hasFaceVertexNormals = OPMutil.GetBit(faceFeatures, 5)
		hasFaceVertexColors = OPMutil.GetBit(faceFeatures, 7)

		pos = 1

		verts = 3
		if isNotTriangle:
			verts = 4


		vertices = []
		normals = []
		uvs = []
		colors = []

		# Build out unique keys
		if verts == 4:
			vertices.append(face[pos + 0])
			vertices.append(face[pos + 1])
			vertices.append(face[pos + 2])
			vertices.append(face[pos + 3])
			pos = pos + 4
		else:
			vertices.append(face[pos + 0])
			vertices.append(face[pos + 1])
			vertices.append(face[pos + 2])
			pos = pos + 3


		pos = pos + 1 #Material Index


		if hasFaceVertexUvs:
			# Assume 1 UV Layer for now
			for uvIndex in range(verts):
				uvs.append(face[pos])
				OPMutil.Print('UV:' + str(face[pos]))
				pos = pos + 1
		else:
			for uvIndex in range(verts):
				uvs.append(-1)

		if hasFaceVertexNormals:
			for normalIndex in range(verts):
				normals.append(face[pos])
				pos = pos + 1
		else:
			for normalIndex in range(verts):
				normals.append(-1)

		if hasFaceVertexColors:
			for colorIndex in range(verts):
				colors.append(face[pos])
				pos = pos + 1
		else:
			for colorIndex in range(verts):
				colors.append(-1)


		tmpfaces = []

		key1 = _getKey(0, vertices, normals, uvs, colors)
		key2 = _getKey(1, vertices, normals, uvs, colors)
		key3 = _getKey(2, vertices, normals, uvs, colors)

		if (local_dict.get(key1) is None):
			local_dict[key1] = local_vertex_count
			local_vertex_list.append(vertices[0])
			local_normal_list.append(normals[0])
			local_uv_list.append(uvs[0])
			local_color_list.append(colors[0])
			tmpfaces.append(local_vertex_count)
			local_vertex_count+=1
		else:
			tmpfaces.append(local_dict[key1])


		if (local_dict.get(key2) is None):
			local_dict[key2] = local_vertex_count
			local_vertex_list.append(vertices[1])
			local_normal_list.append(normals[1])
			local_uv_list.append(uvs[1])
			local_color_list.append(colors[1])
			tmpfaces.append(local_vertex_count)
			local_vertex_count+=1
		else:
			tmpfaces.append(local_dict[key2])

		if (local_dict.get(key3) is None):
			local_dict[key3] = local_vertex_count
			local_vertex_list.append(vertices[2])
			local_normal_list.append(normals[2])
			local_uv_list.append(uvs[2])
			local_color_list.append(colors[2])
			tmpfaces.append(local_vertex_count)
			local_vertex_count+=1
		else:
			tmpfaces.append(local_dict[key3])



		if verts == 4:
			key4 = _getKey(3, vertices, normals, uvs, colors)
			if (local_dict.get(key4) is None):
				local_dict[key4] = local_vertex_count
				local_vertex_list.append(vertices[3])
				local_normal_list.append(normals[3])
				local_uv_list.append(uvs[3])
				local_color_list.append(colors[3])
				tmpfaces.append(local_vertex_count)
				local_vertex_count+=1
			else:
				tmpfaces.append(local_dict[key4])

		local_faces_list.append([tmpfaces[0], tmpfaces[1], tmpfaces[2]])
		if verts == 4:
			local_faces_list.append([tmpfaces[0], tmpfaces[2], tmpfaces[3]])

		OPMutil.Print(face)

	OPMutil.Print(str(len(local_faces_list)))

	OPMbinary.Int(fp, local_vertex_count)
	for i in range(local_vertex_count):
		# OPMutil.Print('Vertex')
		vertInd = local_vertex_list[i]
		OPMutil.Print('VertIndex:' + str(vertInd) + ', ' + str(model['model']['vertices'][vertInd] ) )
		OPMbinary.Vec3( fp, model['model']['vertices'][vertInd] )

		if len(model['model']['normals']) > 0:
			# OPMutil.Print( model['model']['normals'][local_normal_list[i]] )
			OPMbinary.Vec3( fp, model['model']['normals'][local_normal_list[i]] )

		if len(model['model']['uvs']) > 0:
			# OPMutil.Print( model['model']['uvs'][local_uv_list[i]] )
			OPMbinary.Vec2( fp, model['model']['uvs'][local_uv_list[i]] )

		if len(model['model']['colors']) > 0:
			# OPMutil.Print( model['model']['colors'][local_color_list[i]] )
			OPMbinary.Vec3( fp, model['model']['colors'][local_color_list[i]] )
			
		if len(model['model']['indices']) > 0:
			OPMbinary.UShort( fp, model['model']['indices'][vertInd * 4 + 0] )
			OPMbinary.UShort( fp, model['model']['indices'][vertInd * 4 + 1] )
			OPMbinary.UShort( fp, model['model']['indices'][vertInd * 4 + 2] )
			OPMbinary.UShort( fp, model['model']['indices'][vertInd * 4 + 3] )
			
		if len(model['model']['weights']) > 0:
			OPMbinary.Float( fp, model['model']['weights'][vertInd * 4 + 0] )
			OPMbinary.Float( fp, model['model']['weights'][vertInd * 4 + 1] )
			OPMbinary.Float( fp, model['model']['weights'][vertInd * 4 + 2] )
			OPMbinary.Float( fp, model['model']['weights'][vertInd * 4 + 3] )

	OPMutil.Print('Faces')
	OPMutil.Print(str(len(local_faces_list)))
	OPMbinary.UInt(fp, len(local_faces_list))

	for face in local_faces_list:
		OPMbinary.UShort(fp, face[0])
		OPMbinary.UShort(fp, face[1])
		OPMbinary.UShort(fp, face[2])

def _getKey( offset, vertices, normals, uvs, colors):
	return vertices[offset], normals[offset], uvs[offset], colors[offset]

def _writeBones(fp, model, options):
	hierarchy = model['model']['bones']
	OPMbinary.UShort(fp, model['nbone'])
	for i in range(model['nbone']):
		OPMbinary.Short(fp, hierarchy[i][0]) # Index
		OPMbinary.String(fp, hierarchy[i][1]) # Name
		# Position
		OPMbinary.Float(fp, hierarchy[i][2])
		OPMbinary.Float(fp, hierarchy[i][3])
		OPMbinary.Float(fp, hierarchy[i][4])
		# Rotation
		OPMbinary.Float(fp, hierarchy[i][5])
		OPMbinary.Float(fp, hierarchy[i][6])
		OPMbinary.Float(fp, hierarchy[i][7])
		OPMbinary.Float(fp, hierarchy[i][8])
		# Scale
		OPMbinary.Float(fp, hierarchy[i][9])
		OPMbinary.Float(fp, hierarchy[i][10])
		OPMbinary.Float(fp, hierarchy[i][11])


#################################################################
### Public Methods
#################################################################

def Binary( filepath, model, options ):
	OPMutil.Print('Writing to ' + filepath + '...')
	OPMutil.Print('Vertex Count: ' + str(model['nvertex']))
	OPMutil.Print('Face Count: ' + str(model['nface']))
	OPMutil.Print('UV Count: ' + str(model['nuvs']))
	OPMutil.Print('Normal Count: ' + str(model['nnormal']))
	OPMutil.Print('Color Count: ' + str(model['ncolor']))
	OPMutil.Print('Bone Count: ' + str(model['nbone']))
	OPMutil.Print('Bones: ' + str(model['model']['bones']))
	OPMutil.Print('Indices: ' + str(model['model']['indices']))
	OPMutil.Print('Weights: ' + str(model['model']['weights']))



	OPMutil.Print('Faces')
	for face in model['model']['faces']:
		OPMutil.Print(face)

	fp = OPMbinary.OpenFile(filepath)

	OPMbinary.UShort(fp, 1) # Version

	enum_position = 1
	enum_normal = 2
	enum_uv = 4
	enum_tangent = 8
	enum_index = 16
	enum_bones = 32
	enum_skinning = 64
	enum_animations = 128
	enum_color = 256
	
	features = enum_position
	if(options.normals):
		features += enum_normal
	#if(options.Tangents):
	#	features += enum_tangent
	#if(options.uv_coords):
	#	features += enum_uv
	#if(options.colors):
	#	features += enum_color
	if(options.skinning):
		features += enum_bones
		features += enum_skinning

	OPMbinary.UInt(fp, features)

	_writeFaces(fp, model, options)
	_writeBones(fp, model, options)
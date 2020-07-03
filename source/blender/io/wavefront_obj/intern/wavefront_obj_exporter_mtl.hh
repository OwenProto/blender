/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2020 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup obj
 */

#ifndef __WAVEFRONT_OBJ_EXPORTER_MTL_HH__
#define __WAVEFRONT_OBJ_EXPORTER_MTL_HH__

#include "BKE_node_tree_ref.hh"

#include "BLI_path_util.h"

#include "wavefront_obj_exporter_mesh.hh"

namespace blender::io::obj {
class MTLWriter {
 private:
  FILE *mtl_outfile_;
  char mtl_filepath_[PATH_MAX];
  /**
   * One of the object's materials, to be exported.
   */
  Material *export_mtl_;
  /**
   * First Principled-BSDF node encountered in the object's node tree.
   */
  bNode *bsdf_node_;

 public:
  MTLWriter(const char *obj_filepath)
  {
    BLI_strncpy(mtl_filepath_, obj_filepath, PATH_MAX);
    BLI_path_extension_replace(mtl_filepath_, PATH_MAX, ".mtl");
  }

  ~MTLWriter()
  {
    fclose(mtl_outfile_);
  }

  void append_materials(OBJMesh &mesh_to_export);

 private:
  void write_curr_material(const char *object_name);
  void init_bsdf_node(const char *object_name);

  void copy_property_from_node(float *r_property,
                               eNodeSocketDatatype property_type,
                               const bNode *curr_node,
                               const char *identifier);
  void linked_sockets_to_dest_id(Vector<const bke::OutputSocketRef *> &r_linked_sockets,
                                 const bNode *dest_node,
                                 bke::NodeTreeRef &node_tree,
                                 const char *dest_socket_id);
  const bNode *get_node_of_type(Span<const bke::OutputSocketRef *> sockets_list, int sh_node_type);
  const char *get_image_filepath(const bNode *tex_node);
};
}  // namespace blender::io::obj
#endif

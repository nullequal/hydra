import SwiftUI

struct TextureListView: View {
    @State private var refreshID = 0

    @State private var textures: [HydraTextureStorage] = []

    var body: some View {
        ZStack {
            // TODO: support sorting
            Table(self.textures) {
                TableColumn("Dimensions") { texture in
                    Text("\(texture.descriptor.width)x\(texture.descriptor.height)\(texture.descriptor.depth == 1 ? "" : "x\(texture.descriptor.depth)")")
                }
                TableColumn("Type") { texture in
                    Text(texture.descriptor.type.description)
                }
                TableColumn("Format") { texture in
                    Text(texture.descriptor.format.description)
                }
                TableColumn("Levels") { texture in
                    Text("\(texture.descriptor.levelCount)")
                }
                TableColumn("Layers") { texture in
                    Text("\(texture.descriptor.layerCount)")
                }
                TableColumn("Layer Size") { texture in
                    Text("\(texture.descriptor.layerSize)")
                }
                TableColumn("Total Size") { texture in
                    Text("\(texture.descriptor.size)")
                }
            }
                .id("\(refreshID)")  // Unique ID per refresh

            // TODO: add an option to refresh at regular intervals or any time a change happens?
            HStack {
                VStack {
                    Spacer()

                    Button(action: {
                        load()
                        refreshID += 1
                    }) {
                        Image(systemName: "arrow.clockwise")
                            .font(.title2)
                            .foregroundColor(.blue)
                            .padding()
                    }
                    .padding()
                }

                Spacer()
            }
        }
        .onAppear {
            load()
        }
    }

    func load() {
        hydraTextureCacheLock()

        self.textures.removeAll()
        for i in 0..<hydraTextureCacheGetTextureMemoryCount() {
            let mem = hydraTextureCacheGetTextureMemory(at: i)
            for j in 0..<mem.textureGroupCount {
                let group = mem.getTextureGroup(at: j)
                for k in 0..<group.textureStorageCount {
                    let storage = group.getTextureStorage(at: k)
                    self.textures.append(storage)
                }
            }
        }

        hydraTextureCacheUnlock()
    }
}

#include <torch/script.h> // One-stop header.
#include <iostream>
#include <memory>
#include <chrono>

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: example-app <path-to-exported-script-module>\n";
    return -1;
  }

  // Deserialize the ScriptModule from a file using torch::jit::load().
  std::shared_ptr<torch::jit::script::Module> module = torch::jit::load(argv[1]);
  module->to(at::kCUDA);

  assert(module != nullptr);

  for(int i = 0; i < 100; ++i) {
      std::vector<torch::jit::IValue> inputs;
      inputs.push_back(torch::ones({1, 3, 224, 224}).to(at::kCUDA));

      at::Tensor output = module->forward(inputs).toTensor();

      std::cout << output.slice(1, 0, 5) << std::endl;
  }
  return 0;
}

import torch
import torchvision

# An instance of your model.
model = torchvision.models.resnet18(pretrained=True)
model = model.cuda()
model.eval()

# An example input you would normally provide to your model's forward() method.
example = torch.rand(1, 3, 224, 224).cuda()

# Use torch.jit.trace to generate a torch.jit.ScriptModule via tracing.
traced_script_module = torch.jit.trace(model, example)

traced_script_module.save("model.pt")

output = model(torch.ones(1, 3, 224, 224).cuda())
print output[0,:5]

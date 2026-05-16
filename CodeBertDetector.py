import torch.nn as nn
from transformers import AutoModel


class CodeBertDetector(nn.Module):
    def __init__(self, unfreeze_layers=2):
        super().__init__()
        self.bert = AutoModel.from_pretrained("microsoft/codebert-base")
        
        for p in self.bert.parameters():
            p.requires_grad = False
        
        if unfreeze_layers > 0:
            for layer in self.bert.encoder.layer[-unfreeze_layers:]:
                for p in layer.parameters():
                    p.requires_grad = True


        self.classifier = nn.Sequential(
            nn.Linear(768, 256),
            nn.GELU(),
            nn.Dropout(0.5),
            nn.Linear(256, 128),
            nn.GELU(),
            nn.Dropout(0.3),
            nn.Linear(128, 64),
            nn.GELU(),
            nn.Dropout(0.2),
            nn.Linear(64, 2)
        )

    def forward(self, ids, mask):
        out = self.bert(ids, attention_mask=mask).last_hidden_state
        b, s, h = out.shape
        logits = self.classifier(out.view(-1, h))
        return logits.view(b, s, 2)
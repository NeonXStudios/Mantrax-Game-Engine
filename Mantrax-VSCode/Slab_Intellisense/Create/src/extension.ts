import * as vscode from 'vscode';

export function activate(context: vscode.ExtensionContext) {

    const provider = vscode.languages.registerCompletionItemProvider('slab', {

        provideCompletionItems(document: vscode.TextDocument, position: vscode.Position, token: vscode.CancellationToken, context: vscode.CompletionContext) {

            const keywords = [
                'front', 'back', 'param_in', 'param_out', 'param', 'func', 'layer', 'vec4', 'vec3', 'vec2', 'Texture2D', 'mat4'
            ];

            const suggestions = keywords.map(word => {
                const completionItem = new vscode.CompletionItem(word, vscode.CompletionItemKind.Keyword);
                return completionItem;
            });

            return suggestions;
        }
    });

    context.subscriptions.push(provider);
}

export function deactivate() {}

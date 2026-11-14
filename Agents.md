# File Modification Rules

You are working in an existing codebase. Follow these rules strictly whenever modifying files:

1. **Always Read First:** Before modifying any existing file, you MUST call the `read` tool to inspect its current contents.
2. **Use `edit` for Existing Files:** Never use the `write` tool on a file that already exists. Always use the `edit` tool to apply targeted patches.
3. **Preserve All Existing Code:** When adding new functions, imports, or classes, keep all existing code completely intact unless explicitly asked to delete or refactor it.
4. **No Placeholders or Truncation:** Never use comments like `// ... existing code ...` or `# rest of file remains the same`. Include exact surrounding context lines required by the `edit` tool.
5. **Reserve `write` for New Files:** Only use the `write` tool when creating a brand-new file that does not yet exist on disk.
